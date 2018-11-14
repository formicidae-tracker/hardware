#include <arke-avr/systime.h>

#include "LEDs.h"
#include <arke-avr.h>
#include "FanControl.h"


#define DEBOUNCE_TIME_MS 500
#define CRITICAL_REPORT_PERIOD 500
#define CELAENO_RAMP_UP_TIME_MS 1000
#define CELAENO_RAMP_DOWN_TIME_MS 1000

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
		if ( (curtime - (s).debounceStart) >= DEBOUNCE_TIME_MS ) { \
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
	yaacl_txn_t txStatus,txSetPoint;
	ArkeCelaenoSetPoint targetSetPoint;
	ArkeCelaenoSetPoint setPoint;
	ArkeCelaenoStatus status;
	struct Button_t warnLevel,critLevel;
	ArkeSystime_t lastCriticalReport,last;
	enum CelaenoState_t state;
};

struct Celaeno_t C;

void InitCelaeno();

void ProcessCelaeno();

int main() {
	InitLEDs();
	DDRD |= _BV(0) ;
	InitArke((uint8_t*)&C.targetSetPoint,1);

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
	//LEDErrorOff();

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

	//sets

	InitFanControl();
	yaacl_init_txn(&C.txStatus);
	yaacl_init_txn(&C.txSetPoint);
	SetFan1Power(0);
	SetFan2Power(0);

}


void ProcessIncoming() {
	uint8_t length = 0;
	yaacl_idt_t res = ArkeProcess(&length);
	if ( res == ARKE_NO_MESSAGE ) {
		return;
	}
	bool rtr = yaacl_idt_test_rtrbit(res);
	ArkeMessageClass a = (res & 0x1f8) >> 3;
	if ( a == ARKE_CELAENO_SET_POINT && rtr ) {
		if ( yaacl_txn_status(&C.txSetPoint) != YAACL_TXN_PENDING ) {
			ArkeSendCelaenoSetPoint(&C.txSetPoint,false,&C.targetSetPoint);
		}
	}

	if ( a == ARKE_CELAENO_STATUS && rtr ) {
		if ( yaacl_txn_status(&C.txStatus) != YAACL_TXN_PENDING ) {
			ArkeSendCelaenoStatus(&C.txStatus,false,&C.status);
		}
	}
}

void ProcessSensors(ArkeSystime_t now) {
	readout_switch(C.warnLevel,A,0,now);
	readout_switch(C.critLevel,A,1,now);
	uint8_t last = C.status.waterLevel;
	if ( C.warnLevel.value == false && C.critLevel.value == false ) {
		LEDErrorOff();
		C.status.waterLevel = ARKE_CELAENO_NOMINAL;
	} else if ( C.critLevel.value == true && C.warnLevel.value == false ) {
		C.status.waterLevel |= ARKE_CELAENO_RO_ERROR;
		if (C.status.waterLevel != last) {
			LEDErrorBlink(4);
		}
	} else if ( C.critLevel.value == false && C.warnLevel.value == true ) {
		LEDErrorOn();
		C.status.waterLevel = ARKE_CELAENO_WARNING;
	} else {
		C.status.waterLevel = ARKE_CELAENO_WARNING | ARKE_CELAENO_CRITICAL;
		if (C.status.waterLevel != last) {
			LEDErrorBlink(2);
		}
	}
}

void ProcessCelaeno() {
	ArkeSystime_t now = ArkeGetSystime();
	ProcessSensors(now);
	FanControlStatus_e s = ProcessFanControl();
	C.status.fanStatus = GetFan1RPM() | ( (s & FAN_1_STALL) != 0 ? 0x8000 : 0);
	ProcessIncoming();

	//now process the state machine
	if (C.state < NB_CELAENO_STATES ) {
		C.state = stateFunctions[C.state](now);
	}

	bool shouldReport = ( (s & FAN_1_STALL) != 0 ||
	                      (C.status.waterLevel > ARKE_CELAENO_WARNING && C.targetSetPoint.Power > 0 ) );
	if ( !shouldReport || yaacl_txn_status(&C.txStatus) != YAACL_TXN_UNSUBMITTED ) {
		return;
	}

	if ( (now - C.lastCriticalReport) < CRITICAL_REPORT_PERIOD ) {
		return;
	}
	C.lastCriticalReport = now;
	//send the alert on the bus
	ArkeSendCelaenoStatus(&C.txStatus,true,&C.status);
}

#define water_level_critical_error() (C.status.waterLevel >= 0x02)

enum CelaenoState_t CelaenoOffState(ArkeSystime_t now) {
	if ( water_level_critical_error() ) {
		return CELAENO_OFF;
	}
	if ( C.targetSetPoint.Power > 0 ) {
		C.last = now;
		C.setPoint = C.targetSetPoint;
		SetFan1Power(C.setPoint.Power);
		return CELAENO_RAMPUP;
	}
	return CELAENO_OFF;
}

enum CelaenoState_t CelaenoOnState(ArkeSystime_t now) {
	if ( water_level_critical_error() || C.targetSetPoint.Power == 0 ) {
		relay_off();
		C.last = now;
		LEDReadyPulse();
		return CELAENO_RAMPDOWN;
	}

	if (C.targetSetPoint.Power != C.setPoint.Power ) {
		C.setPoint.Power = C.targetSetPoint.Power;
		SetFan1Power(C.setPoint.Power);
	}

	return CELAENO_ON;
}

enum CelaenoState_t CelaenoRampUpState(ArkeSystime_t now) {
	if ( water_level_critical_error() || C.targetSetPoint.Power == 0 ) {
		SetFan1Power(0);
		C.setPoint.Power = 0;
		return CELAENO_OFF;
	}

	if ( (now - C.last) >= CELAENO_RAMP_UP_TIME_MS ) {
		relay_on();
		LEDReadyOn();
		return CELAENO_ON;
	}

	return CELAENO_RAMPUP;

}
enum CelaenoState_t CelaenoRampDownState(ArkeSystime_t now) {
	if ( (now - C.last) < CELAENO_RAMP_DOWN_TIME_MS ) {
		return CELAENO_RAMPDOWN;
	}
	C.setPoint.Power = 0;
	SetFan1Power(0);
	return CELAENO_OFF;
}
