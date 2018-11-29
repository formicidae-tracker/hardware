#include "Sensors.h"

#include <arke-avr/systime.h>
#include <yaail.h>

#define SENSOR_LOOP_PERIOD_MS 200
#define HIH6130_CONVERSION_TIME_MS 50
#define TMP1075_MAX_TRIALS 4


#define HIH6130_I2C_ADDRESS 0x27
#define TMP1075_I2C_ADDRESS 0x48

typedef enum {
	SENSOR_IDLE = 0,
	SENSOR_WAIT_FOR_CONVERSION = 1,
	SENSOR_WAIT_FOR_RESULT = 2,
	NB_SENSOR_STATES
} SensorState_e;

typedef SensorState_e (*StateFunction)( bool *);

SensorState_e SensorProcessIdle(bool*);
SensorState_e SensorProcessWaitForConversion(bool*);
SensorState_e SensorProcessWaitForResult(bool*);

StateFunction stateFunctions[NB_SENSOR_STATES] = {
	&SensorProcessIdle,
	&SensorProcessWaitForConversion,
	&SensorProcessWaitForResult
};

typedef struct {
	ArkeZeusReport report;
	yaail_txn_t hih6130,tmp1075[3];
	uint8_t tries[3];
	uint8_t hih6130Data[4];
	uint8_t tmp1075Data[6];
	ArkeSystime_t last;
	SensorState_e state;
	bool noNewData;
} Sensors_t;

Sensors_t S;


void InitSensors() {
	yaail_init_txn(&S.hih6130);
	for ( uint8_t i = 0 ; i < 3; ++i ) {
		yaail_init_txn(&S.tmp1075[i]);
	}
	S.last = 0;
	S.state = SENSOR_IDLE;
	S.report.Humidity = 0x3fff;
	S.report.Temperature1 = 0x3fff;
	S.report.Temperature2 = 0x0fff;
	S.report.Temperature3 = 0x0fff;
	S.report.Temperature4 = 0x0fff;
}



bool ProcessSensors() {
	bool toReturn;
	S.state = stateFunctions[S.state](&toReturn);
	return toReturn;
}

const ArkeZeusReport * GetSensorData() {
	return &S.report;
}

SensorState_e SensorProcessIdle(bool* ret) {
	*ret = false;
	ArkeSystime_t now = ArkeGetSystime();
	if ( (now-S.last) < SENSOR_LOOP_PERIOD_MS ) {
		return SENSOR_IDLE;
	}
	S.last = now;
	for(uint8_t i = 0; i < 3; ++i ) {
		S.tmp1075Data[2*i] = 0x00;
		yaail_write_and_read(&S.tmp1075[i],
		                     TMP1075_I2C_ADDRESS + i,
		                     &(S.tmp1075Data[2*i]),
		                     1,
		                     2);
		S.tries[i] = 1;
	}
	yaail_write(&S.hih6130,HIH6130_I2C_ADDRESS,0,0);
	return SENSOR_WAIT_FOR_CONVERSION;
}
SensorState_e SensorProcessWaitForConversion(bool* ret) {
	*ret = false;
	ArkeSystime_t now = ArkeGetSystime();
	for ( uint8_t i = 0; i < 3 ; ++i ) {
		yaail_txn_status_e s = yaail_txn_status(&S.tmp1075[i]);
		if ( yaail_txn_status_has_error(s) && S.tries[i] < TMP1075_MAX_TRIALS ) {
			S.tmp1075Data[2*i]  = 0x00;
			yaail_write_and_read(&S.tmp1075[i],
			                     TMP1075_I2C_ADDRESS + i,
			                     &S.tmp1075Data[2*i],
			                     1,
			                     2);
			S.tries[i] += 1;
		}
	}

	if ( (now-S.last) < HIH6130_CONVERSION_TIME_MS ) {
		return SENSOR_WAIT_FOR_CONVERSION;
	}
	if (yaail_txn_status(&S.hih6130) == YAAIL_DONE ) {
		yaail_read(&S.hih6130,HIH6130_I2C_ADDRESS,S.hih6130Data,4);
	}
	return SENSOR_WAIT_FOR_RESULT;
}

SensorState_e SensorProcessWaitForResult(bool* ret) {
	*ret = false;
	yaail_txn_status_e s = yaail_txn_status(&S.hih6130);
	if( s == YAAIL_PENDING || s == YAAIL_SCHEDULED) {
		return SENSOR_WAIT_FOR_RESULT;
	}

	if ( s == YAAIL_DONE) {
		*ret = true;
		if ( (S.hih6130Data[0] & 0xc0) != 0x00 ) {
			S.report.Humidity = 0x3fff;
			S.report.Temperature1 = 0x3fff;
		} else {
			S.report.Humidity = S.hih6130Data[1] | ( ( (uint16_t)(S.hih6130Data[0] & 0x3f) ) << 8 );
			S.report.Temperature1 = (S.hih6130Data[3] >> 2) | ( ((uint16_t)S.hih6130Data[2]) << 6);
		}

	} else {
		S.report.Humidity = 0x3fff;
		S.report.Temperature1 = 0x3fff;
	}

	uint16_t results[3];

	for ( uint8_t i = 0; i < 3; ++i ) {
		if ( yaail_txn_status(&S.tmp1075[i]) != YAAIL_DONE ) {
			results[i] = 0x0fff;
			continue;
		}
		results[i] = (S.tmp1075Data[2*i+1] >> 4) | ( ((uint16_t)S.tmp1075Data[2*i]) << 4 );
	}

#define update_report(i) do{\
		S.report.Temperature ## i = results[i-2]; \
	}while(0)
	update_report(2);
	update_report(3);
	update_report(4);
#undef udpate_report

	return SENSOR_IDLE;
}
