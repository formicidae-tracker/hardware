#include "Sensors.h"
#include "zeus/src/crc8.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <string.h>

#include <arke-avr.h>
#include <arke-avr/systime.h>

#include <yaail.h>

#define SENSOR_LOOP_PERIOD_MS     500
#define SHT41I_CONVERSION_TIME_MS 20
#define TMP1075_MAX_TRIALS        4

#define SHT41I_I2C_ADDRESS  0x44
#define TMP1075_I2C_ADDRESS 0x48

typedef enum {
	SENSOR_IDLE                = 0,
	SENSOR_WAIT_FOR_CONVERSION = 1,
	SENSOR_WAIT_FOR_RESULT     = 2,
	NB_SENSOR_STATES
} SensorState_e;

ArkeZeusDeltaTemperature EEMEM EEDeltas  = {
	.Delta = {0,0,0,0},
};

typedef SensorState_e (*StateFunction)( bool *, ArkeSystime_t now);

SensorState_e SensorProcessIdle(bool*,ArkeSystime_t now);
SensorState_e SensorProcessWaitForConversion(bool*,ArkeSystime_t now);
SensorState_e SensorProcessWaitForResult(bool*,ArkeSystime_t now);

StateFunction stateFunctions[NB_SENSOR_STATES] = {
	&SensorProcessIdle,
	&SensorProcessWaitForConversion,
	&SensorProcessWaitForResult
};

typedef struct {
	ArkeZeusReport           report;
	yaail_txn_t              sht41i, tmp1075[3];
	uint8_t                  tries[3];
	uint8_t                  sht41iData[6];
	uint8_t                  tmp1075Data[6];
	ArkeSystime_t            last;
	SensorState_e            state;
	bool                     noNewData;
	ArkeZeusDeltaTemperature Deltas;
} Sensors_t;

Sensors_t S;

void InitSensors() {
	yaail_init_txn(&S.sht41i);
	for (uint8_t i = 0; i < 3; ++i) {
		yaail_init_txn(&S.tmp1075[i]);
	}
	S.last                = 0;
	S.state               = SENSOR_IDLE;
	S.report.Humidity     = 0x3fff;
	S.report.Temperature1 = 0x3fff;
	S.report.Temperature2 = 0x0fff;
	S.report.Temperature3 = 0x0fff;
	S.report.Temperature4 = 0x0fff;

	uint8_t sreg = SREG;
	cli();
	eeprom_read_block(&(S.Deltas), &EEDeltas, sizeof(ArkeZeusDeltaTemperature));
	SREG = sreg;
}

bool ProcessSensors(ArkeSystime_t now) {
	bool toReturn;
	// fights memory corruptions and bugs
	if (S.state < 0 || S.state >= NB_SENSOR_STATES) {
		// Resets the sensor loop
		S.state = SENSOR_IDLE;
		S.last  = ArkeGetSystime();
	}

	S.state = stateFunctions[S.state](&toReturn, now);
	return toReturn;
}

const ArkeZeusReport * GetSensorData() {
	return &S.report;
}

SensorState_e SensorProcessIdle(bool *ret, ArkeSystime_t now) {
	*ret = false;
	if ((now - S.last) < SENSOR_LOOP_PERIOD_MS) {
		return SENSOR_IDLE;
	}
	S.last = now;
	for (uint8_t i = 0; i < 3; ++i) {
		S.tmp1075Data[2 * i] = 0x00;
		yaail_write_and_read(
		    &S.tmp1075[i],
		    TMP1075_I2C_ADDRESS + i,
		    &(S.tmp1075Data[2 * i]),
		    1,
		    2
		);
		S.tries[i] = 1;
	}
	S.sht41iData[0] = 0xfd;
	yaail_write(&S.sht41i, SHT41I_I2C_ADDRESS, S.sht41iData, 1);
	return SENSOR_WAIT_FOR_CONVERSION;
}

SensorState_e SensorProcessWaitForConversion(bool *ret, ArkeSystime_t now) {
	*ret = false;
	for (uint8_t i = 0; i < 3; ++i) {
		yaail_txn_status_e s = yaail_txn_status(&S.tmp1075[i]);
		if (yaail_txn_status_has_error(s) && S.tries[i] < TMP1075_MAX_TRIALS) {
			S.tmp1075Data[2 * i] = 0x00;
			yaail_write_and_read(
			    &S.tmp1075[i],
			    TMP1075_I2C_ADDRESS + i,
			    &S.tmp1075Data[2 * i],
			    1,
			    2
			);
			S.tries[i] += 1;
		}
	}

	if ((now - S.last) < SHT41I_CONVERSION_TIME_MS) {
		return SENSOR_WAIT_FOR_CONVERSION;
	}
	if (yaail_txn_status(&S.sht41i) == YAAIL_DONE) {
		yaail_read(
		    &S.sht41i,
		    SHT41I_I2C_ADDRESS,
		    S.sht41iData,
		    sizeof(S.sht41iData)
		);
	}
	return SENSOR_WAIT_FOR_RESULT;
}

#define add_delta(target, delta, low, high)                                    \
	do {                                                                       \
		if ((delta) > 0 && (target) > ((high) - (delta))) {                    \
			(target) = (high);                                                 \
		} else if ((delta) < 0 && (target) < ((low) - (delta))) {              \
			(target) = (low);                                                  \
		} else {                                                               \
			(target) += (delta);                                               \
		}                                                                      \
	} while (0)

bool checkSHT41ICRC(const uint8_t *buffer, size_t len) {
	uint8_t crc = 0xff;
	for (int i = 0; i < len; ++i) {
		crc = CRC8_AppendByte(crc, 0x31, buffer[i]);
	}
	return crc == 0x00;
}

#define max_(a, b)          ((a) > (b) ? (a) : (b))
#define min_(a, b)          ((a) < (b) ? (a) : (b))
#define clamp_(v, low, high) min_(max_((v), (low)), (high))

SensorState_e SensorProcessWaitForResult(bool *ret, ArkeSystime_t now) {
	*ret                 = false;
	yaail_txn_status_e s = yaail_txn_status(&S.sht41i);
	if (s == YAAIL_PENDING || s == YAAIL_SCHEDULED) {
		return SENSOR_WAIT_FOR_RESULT;
	}

	if (s == YAAIL_DONE) {
		*ret = true;

		if (checkSHT41ICRC(&S.sht41iData[0], 3) == false ||
		    checkSHT41ICRC(&S.sht41iData[3], 3) == false) {
			ArkeReportError(0x001f);
			S.report.Humidity     = 0x3fff;
			S.report.Temperature1 = 0x3fff;
		} else {
			uint32_t tempTicks =
			    ((uint16_t)(S.sht41iData[0]) << 8) | S.sht41iData[1];
			uint32_t rhTicks =
			    ((uint16_t)(S.sht41iData[3]) << 8) | S.sht41iData[4];
			S.report.Temperature1 =
			    clamp_(((tempTicks * 35) / 128) - 512, 0, 0x3ffe);
			S.report.Humidity = clamp_(((rhTicks * 5) / 16) - 983, 0, 0x3ffe);
		}

	} else {
		ArkeReportError(0x0010 | s);
		S.report.Humidity     = 0x3fff;
		S.report.Temperature1 = 0x3fff;
	}

	int16_t results[3];

	for (uint8_t i = 0; i < 3; ++i) {
		if (yaail_txn_status(&S.tmp1075[i]) != YAAIL_DONE) {
			results[i] = 0x0fff;
			continue;
		}
		results[i] = (S.tmp1075Data[2 * i + 1] >> 4) |
		             (((uint16_t)S.tmp1075Data[2 * i]) << 4);
		if ((results[i] & 0x0800) != 0) {
			results[i] |= 0xf000;
		}
	}

#define update_report(i)                                                       \
	do {                                                                       \
		add_delta(                                                             \
		    results[i - 2],                                                    \
		    S.Deltas.Delta[i - 1],                                             \
		    (int16_t)0xf800,                                                   \
		    (int16_t)0x07ff                                                    \
		);                                                                     \
		S.report.Temperature##i = results[i - 2];                              \
	} while (0)
	update_report(2);
	update_report(3);
	update_report(4);
#undef udpate_report

	return SENSOR_IDLE;
}

void SensorsSetDeltaTemperature(const ArkeZeusDeltaTemperature * deltas) {
	memcpy(&(S.Deltas),deltas, sizeof(ArkeZeusDeltaTemperature));
	uint8_t sreg = SREG;
	cli();
	eeprom_update_block(&(S.Deltas),&EEDeltas,sizeof(ArkeZeusDeltaTemperature));
	SREG = sreg;
}

const ArkeZeusDeltaTemperature * SensorsGetDeltaTemperature() {
	return &(S.Deltas);
}


void SensorsReset() {
	S.state = NB_SENSOR_STATES;
}
