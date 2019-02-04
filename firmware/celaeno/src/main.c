#include <arke-avr/systime.h>
#include "LEDs.h"
#include <arke-avr.h>
#include "FanControl.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <yaail.h>


ArkeCelaenoConfig EEMEM eepromConfig = {
	.RampUpTimeMS = 250,
	.RampDownTimeMS = 3000,
	.MinOnTimeMS = 1500,
	.DebounceTimeMS = 1000
};

#define CLEANING_POWER 0x40

#define CRITICAL_REPORT_PERIOD 500

implements_ArkeSoftwareReset()

struct Button_t {
	ArkeSystime_t debounceStart;
	bool value,lastValue;
};

#define init_switch(s,port,pin) do {	  \
		(s).value = false; \
		(s).lastValue = false; \
		DDR ## port &= ~_BV(pin); \
	}while(0)

#define readout_switch(s,port,pin,curtime) do {	  \
		bool reading = (PIN ## port & _BV(pin)) != 0 ; \
		if ( reading != (s).lastValue ) { \
			(s).debounceStart = curtime; \
			(s).lastValue = reading; \
			break; \
		} \
		if ( (curtime - (s).debounceStart) >= C.config.DebounceTimeMS ) { \
			(s).value = reading; \
		} \
	}while(0)

#define init_relay() do{ \
		DDRB |= _BV(2); \
	}while(0)
#define relay_on() do{\
		PORTB |= _BV(2); \
	}while(0)
#define relay_off() do{\
		PORTB &= ~_BV(2); \
	}while(0)

#define relay_is_on() ( (PORTB & _BV(2)) != 0 )
#define relay_is_off() ( (PORTB & _BV(2)) == 0 )

enum CelaenoState_t {
	CELAENO_OFF = 0,
	CELAENO_ON = 1,
	CELAENO_RAMPUP = 2,
	CELAENO_RAMPDOWN = 3,
	NB_CELAENO_STATES = 4,
	CELAENO_SEND_ALERT = 0x80
};

typedef enum CelaenoState_t (*StateFnct_t)(ArkeSystime_t);

enum CelaenoState_t CelaenoOffState(ArkeSystime_t now);
enum CelaenoState_t CelaenoOnState(ArkeSystime_t now);
enum CelaenoState_t CelaenoRampUpState(ArkeSystime_t now);
enum CelaenoState_t CelaenoRampDownState(ArkeSystime_t now);


StateFnct_t stateFunctions[NB_CELAENO_STATES] = {
	&CelaenoOffState,
	&CelaenoOnState,
	&CelaenoRampUpState,
	&CelaenoRampDownState
};

struct Celaeno_t {
	union {
		ArkeCelaenoConfig config;
		struct __attribute__((packed)){
			uint8_t Power;
			uint8_t ignored[7];
		} setPoint;
		uint8_t bytes[8];
	} inBuffer;
	yaacl_txn_t txStatus,txSetPoint,txConfig;
	ArkeCelaenoSetPoint targetSetPoint;
	ArkeCelaenoSetPoint setPoint;
	ArkeCelaenoConfig   config;
	ArkeCelaenoStatus status;
	struct Button_t warnLevel,critLevel;
	ArkeSystime_t lastCriticalReport,last;
	enum CelaenoState_t state;
	bool lockOn;
	bool lockOff;
};

struct Celaeno_t C;

void InitCelaeno();

void ProcessCelaeno();

int main() {
	InitLEDs();
	DDRD |= _BV(0) ;
	InitArke((uint8_t*)&C.inBuffer,8);

	InitCelaeno();

	while(true) {
		ProcessLEDs();
		ProcessCelaeno();
	}

}

void InitCelaeno() {
	init_relay();
	relay_off();
	LEDReadyPulse();
	LEDErrorOff();

	C.state = CELAENO_OFF;
	C.setPoint.Power = 0;
	C.targetSetPoint.Power = 0;
	C.status.waterLevel = ARKE_CELAENO_NOMINAL;
	C.status.fanStatus = 0x0000;
	init_switch(C.warnLevel,A,0);
	init_switch(C.critLevel,A,1);
	C.lastCriticalReport = 0;
	C.last = 0;
	C.state = CELAENO_OFF;
	C.lockOff = false;
	//sets
	yaail_init(YAAIL_25);
	InitFanControl();
	yaacl_init_txn(&C.txStatus);
	yaacl_init_txn(&C.txSetPoint);
	yaacl_init_txn(&C.txConfig);
	SetFan1Power(0);
	SetFan2Power(0);

	cli();
	eeprom_read_block(&C.config,&eepromConfig,sizeof(ArkeCelaenoConfig));
	sei();
}


void ProcessIncoming() {
	uint8_t length = 0;
	yaacl_idt_t res = ArkeProcess(&length);
	if ( res == ARKE_NO_MESSAGE ) {
		return;
	}
	bool rtr = yaacl_idt_test_rtrbit(res);
	ArkeMessageClass a = (res & 0x1f8) >> 3;
	if ( a == ARKE_CELAENO_SET_POINT) {
		if ( rtr && yaacl_txn_status(&C.txSetPoint) != YAACL_TXN_PENDING ) {
			ArkeSendCelaenoSetPoint(&C.txSetPoint,false,&C.targetSetPoint);
			return;
		}

		if (!rtr && length == 1) {
			C.targetSetPoint.Power = C.inBuffer.setPoint.Power;
			return;
		}
		return;
	}

	if ( a == ARKE_CELAENO_STATUS && rtr ) {
		if ( yaacl_txn_status(&C.txStatus) != YAACL_TXN_PENDING ) {
			ArkeSendCelaenoStatus(&C.txStatus,false,&C.status);
		}
		return;
	}

	if ( a == ARKE_CELAENO_CONFIG) {
		if (rtr && yaacl_txn_status(&C.txConfig) != YAACL_TXN_PENDING ) {
			ArkeSendCelaenoConfig(&C.txConfig,false,&C.config);
			return;
		}
		if (!rtr && length == 8 ) {
			for (uint8_t i = 0; i < 4; ++i) {
				uint16_t * actual = (uint16_t*)&C.config + i;
				uint16_t * target = (uint16_t*)&C.inBuffer.bytes + i;
				uint16_t * eeprom = (uint16_t*)&eepromConfig + i;
				if ( *actual != *target) {
					*actual = *target;
					uint8_t sreg = SREG;
					cli();
					eeprom_write_word(eeprom,*target);
					SREG = sreg;
				}
			}
			return;
		}

		return;
	}

}

void ProcessSensors(ArkeSystime_t now) {
	readout_switch(C.warnLevel,A,0,now);
	readout_switch(C.critLevel,A,1,now);
	if ( C.warnLevel.value == false && C.critLevel.value == false ) {
		C.status.waterLevel = ARKE_CELAENO_NOMINAL;
	} else if ( C.critLevel.value == true && C.warnLevel.value == false ) {
		C.status.waterLevel |= ARKE_CELAENO_RO_ERROR;
	} else if ( C.critLevel.value == false && C.warnLevel.value == true ) {
		C.status.waterLevel = ARKE_CELAENO_WARNING;
	} else {
		C.status.waterLevel = ARKE_CELAENO_WARNING | ARKE_CELAENO_CRITICAL;
	}
}

void SetLED() {
	if ( ArkeFanStall(C.status) != 0  ) {
		LEDErrorBlink(4);
		return;
	}

	if ( (C.status.waterLevel & ARKE_CELAENO_RO_ERROR) != 0) {
		LEDErrorBlink(3);
		return;
	}

	if ( (C.status.waterLevel >= ARKE_CELAENO_CRITICAL) ) {
		LEDErrorBlink(2);
		return;
	}

	if( C.status.waterLevel == ARKE_CELAENO_WARNING ) {
		LEDErrorOn();
		return;
	}

	LEDErrorOff();
}

void ProcessCelaeno() {
	bool shouldReport = false;
	ArkeSystime_t now = ArkeGetSystime();
	ProcessSensors(now);
	if (C.status.waterLevel >= ARKE_CELAENO_WARNING && C.targetSetPoint.Power > 0 ) {
		shouldReport = true;
	}
	ProcessFanControl();
	C.status.fanStatus = GetFan1RPM();
	SetLED();

	ProcessIncoming();

	//now process the state machine
	if (C.state < NB_CELAENO_STATES ) {
		enum CelaenoState_t newState = stateFunctions[C.state](now);
		if ( newState != C.state ) {
			C.last = now;
			C.state = newState;
		}
	}

	if ( !shouldReport || yaacl_txn_status(&C.txStatus) != YAACL_TXN_UNSUBMITTED ) {
		return;
	}

	if ( (now - C.lastCriticalReport) < CRITICAL_REPORT_PERIOD ) {
		return;
	}
	C.lastCriticalReport = now;
	ArkeSendCelaenoStatus(&C.txStatus,true,&C.status);
}

#define water_level_critical_error() (C.status.waterLevel >= 0x02)
#define fan_critical_error() ( (C.status.fanStatus & ARKE_FAN_STALL_ALERT) != 0)


enum CelaenoState_t CelaenoOffState(ArkeSystime_t now) {
	if ( C.lockOff == true ) {
		if ( ( now - C.last ) >= 2*C.config.MinOnTimeMS ) {
			C.lockOff = false;
		} else {
			return CELAENO_OFF;
		}
	}
	if ( water_level_critical_error() ) {
		return CELAENO_OFF;
	}
	if ( C.targetSetPoint.Power > 0 ) {
		C.setPoint = C.targetSetPoint;
		SetFan1Power(C.setPoint.Power);
		SetFan2Power(C.setPoint.Power);
		return CELAENO_RAMPUP;
	}
	return CELAENO_OFF;
}

enum CelaenoState_t CelaenoOnState(ArkeSystime_t now) {
	if ( C.lockOn ) {
		//to protect the relay, we avoid to switch it on back and
		//forth unless a minimum on time is met.
		if ( (now - C.last ) >= C.config.MinOnTimeMS ) {
			C.lockOn = false;
		} else {
			return CELAENO_ON;
		}
	}

	// We should not go on a rampdown / off state on a fan stall
	// condition, otherwise the power would go to zero and therefore
	// the condition would be cleared.
	if ( relay_is_on() && fan_critical_error() ) {
		relay_off();
	} else if ( relay_is_off() && !fan_critical_error() ) {
		relay_on();
	}

	if ( water_level_critical_error()
	     || C.targetSetPoint.Power == 0 ) {
		relay_off();
		LEDReadyPulse();
		SetFan1Power(CLEANING_POWER);
		SetFan2Power(CLEANING_POWER);
		return CELAENO_RAMPDOWN;
	}

	if (C.targetSetPoint.Power != C.setPoint.Power ) {
		C.setPoint.Power = C.targetSetPoint.Power;
		SetFan1Power(C.setPoint.Power);
		SetFan2Power(C.setPoint.Power);
	}

	return CELAENO_ON;
}

enum CelaenoState_t CelaenoRampUpState(ArkeSystime_t now) {
	if ( water_level_critical_error()
	     || C.targetSetPoint.Power == 0 ) {
		SetFan1Power(0);
		SetFan2Power(0);
		C.setPoint.Power = 0;
		C.lockOff = true;
		return CELAENO_OFF;
	}

	if ( (now - C.last) >= C.config.RampUpTimeMS ) {
		relay_on();
		LEDReadyOn();
		C.lockOn = true;
		return CELAENO_ON;
	}

	return CELAENO_RAMPUP;

}
enum CelaenoState_t CelaenoRampDownState(ArkeSystime_t now) {
	if ( (now - C.last) < C.config.RampDownTimeMS ) {
		return CELAENO_RAMPDOWN;
	}

	C.setPoint.Power = 0;
	SetFan1Power(0);
	SetFan2Power(0);
	C.lockOff = true;
	return CELAENO_OFF;
}
