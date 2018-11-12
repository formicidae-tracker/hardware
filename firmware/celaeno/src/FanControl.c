#include "FanControl.h"

#include <avr/pgmspace.h>

#include <yaail.h>
#include <emc2302.h>
#include "LEDs.h"
#include <arke-avr/systime.h>

#define FC_UPDATE_PERIOD 200
#define FC_IGNORE_ALERT_TIME 400


const PROGMEM uint16_t RPMLookUp[255]  = {0,61440,30720,20480,15360,12288,10240,8777,7680,6827,6144,5585,5120,4726,4389,4096,3840,3614,3413,3234,3072,2926,2793,2671,2560,2458,2363,2276,2194,2119,2048,1982,1920,1862,1807,1755,1707,1661,1617,1575,1536,1499,1463,1429,1396,1365,1336,1307,1280,1254,1229,1205,1182,1159,1138,1117,1097,1078,1059,1041,1024,1007,991,975,960,945,931,917,904,890,878,865,853,842,830,819,808,798,788,778,768,759,749,740,731,723,714,706,698,690,683,675,668,661,654,647,640,633,627,621,614,608,602,597,591,585,580,574,569,564,559,554,549,544,539,534,530,525,521,516,512,508,504,500,495,492,488,484,480,476,473,469,465,462,459,455,452,448,445,442,439,436,433,430,427,424,421,418,415,412,410,407,404,402,399,396,394,391,389,386,384,382,379,377,375,372,370,368,366,364,361,359,357,355,353,351,349,347,345,343,341,339,338,336,334,332,330,329,327,325,323,322,320,318,317,315,313,312,310,309,307,306,304,303,301,300,298,297,295,294,293,291,290,288,287,286,284,283,282,281,279,278,277,276,274,273,272,271,269,268,267,266,265,264,263,261,260,259,258,257,256,255,254,253,252,251,250,249,248,247,246,245,244,243,242};

enum DataBufferByteIdx {
	FC_FAN1_SP_ID = 0,
	FC_FAN1_SP = 0,
	FC_FAN1_SP_VALUE = 1,
	FC_FAN2_SP_ID = 1,
	FC_FAN2_SP = 2,
	FC_FAN2_SP_VALUE = 3,
	FC_TACH1_COUNT_ID = 2,
	FC_TACH1_COUNT = 4,
	FC_TACH1_COUNT_VALUE = 4,
	FC_TACH2_COUNT_ID = 3,
	FC_TACH2_COUNT = 5,
	FC_TACH2_COUNT_VALUE = 5,
	FC_FAN_STALL_ID = 4,
	FC_FAN_STALL = 6,
	FC_FAN_STALL_VALUE = 6,
	FC_NB_YAAIL_TXN = 5,
	FC_DATA_BUFFER_SIZE = 7,
};


typedef struct {
	ArkeSystime_t last;
	FanControlStatus_e status;
	uint16_t RPM[2];
	uint8_t data[FC_DATA_BUFFER_SIZE];
	yaail_txn_t tx[FC_NB_YAAIL_TXN];
	bool ignoreFailureAtStart;
} FanControl_t;
FanControl_t FC;


void InitFanControl() {
	yaail_init(YAAIL_100);
	for (uint8_t i = 0; i < FC_NB_YAAIL_TXN; ++i) {
		yaail_init_txn(&FC.tx[i]);
	}

	FC.last = 0;
	FC.RPM[0] = 0;
	FC.RPM[1] = 1;
	FC.data[FC_FAN1_SP] = EMC_FAN1_SET_POINT_REG;
	FC.data[FC_FAN1_SP_VALUE] = 0x00;
	FC.data[FC_FAN2_SP] = EMC_FAN2_SET_POINT_REG;
	FC.data[FC_FAN2_SP_VALUE] = 0x00;
	FC.data[FC_TACH1_COUNT] = EMC_FAN1_TACH_COUNT_REG_H;
	FC.data[FC_TACH2_COUNT] = EMC_FAN2_TACH_COUNT_REG_H;
	FC.data[FC_FAN_STALL] = EMC_FAN_STALL_REG;

	#define NB_CONF 10

	uint8_t registers_and_data[2*NB_CONF] = {EMC_FAN1_SET_POINT_REG,0x00,
	                                         EMC_FAN2_SET_POINT_REG,0x00,
	                                         EMC_FAN1_CONF1_REG, EMC_DIS_ALGO | EMC_RANGE_500 | EMC_EDGES_3 | EMC_UPDATE_200MS,
	                                         EMC_FAN2_CONF1_REG, EMC_DIS_ALGO | EMC_RANGE_500 | EMC_EDGES_3 | EMC_UPDATE_200MS,
	                                         EMC_FAN1_CONF2_REG, 0x68,
	                                         EMC_FAN2_CONF2_REG, 0x68,
	                                         EMC_FAN1_SPIN_UP_REG,EMC_DRIVE_FAIL_COUNT_0 | EMC_ENABLE_KICK | EMC_SPIN_TIME_1000MS | EMC_SPIN_LVL_65,
	                                         EMC_FAN2_SPIN_UP_REG,EMC_DRIVE_FAIL_COUNT_0 | EMC_ENABLE_KICK | EMC_SPIN_TIME_1000MS | EMC_SPIN_LVL_65,
	                                         EMC_FAN1_VALID_TACH_REG,0xfe,
	                                         EMC_FAN2_VALID_TACH_REG,0xfe,
	};

	FC.status = FAN_OK;
	yaail_txn_t txn[NB_CONF];


	for (uint8_t i = 0; i < NB_CONF; ++i ) {
		yaail_error_e err = yaail_write(&txn[i],EMC2302_1_I2C_ADDRESS,&registers_and_data[2*i],2);
		if ( err != YAAIL_NO_ERROR) {
			LEDErrorBlink(3);
		}
	}

	yaail_spin_until_done(&txn[NB_CONF-1]);
	FC.ignoreFailureAtStart = true;
}

uint16_t tach_to_RPM(uint8_t highTachByte) {
	if ( highTachByte == 0xff ) {
		return 0;
	}
	return pgm_read_word(&(RPMLookUp[highTachByte]));
}



FanControlStatus_e ProcessFanControl() {

	ArkeSystime_t now = ArkeGetSystime();
	if ( (now - FC.last) >= FC_UPDATE_PERIOD ) {
		FC.data[FC_TACH1_COUNT] = EMC_FAN1_TACH_COUNT_REG_H;
		FC.data[FC_TACH2_COUNT] = EMC_FAN2_TACH_COUNT_REG_H;
		FC.data[FC_FAN_STALL] = EMC_FAN_STALL_REG;
		yaail_write_and_read(&(FC.tx[FC_TACH1_COUNT_ID]),EMC2302_1_I2C_ADDRESS,&FC.data[FC_TACH1_COUNT],1,1);
		yaail_write_and_read(&(FC.tx[FC_TACH2_COUNT_ID]),EMC2302_1_I2C_ADDRESS,&FC.data[FC_TACH2_COUNT],1,1);
		yaail_write_and_read(&(FC.tx[FC_FAN_STALL_ID]),EMC2302_1_I2C_ADDRESS,&FC.data[FC_FAN_STALL],1,1);
		FC.last = now;
	}
	if ( FC.ignoreFailureAtStart && now >= FC_IGNORE_ALERT_TIME) {
		FC.ignoreFailureAtStart = false;
	}


	if ( yaail_txn_status(&FC.tx[FC_TACH1_COUNT_ID]) == YAAIL_DONE ) {
		//This is ok since we are done, but we wont lookup in loop once done
		FC.tx[FC_TACH1_COUNT_ID] = YAAIL_PENDING;
		FC.RPM[0] = tach_to_RPM(FC.data[FC_TACH1_COUNT_VALUE]);
	}

	if ( yaail_txn_status(&FC.tx[FC_TACH2_COUNT_ID]) == YAAIL_DONE ) {
		FC.tx[FC_TACH2_COUNT_ID] = YAAIL_PENDING;
		FC.RPM[1] = tach_to_RPM(FC.data[FC_TACH2_COUNT_VALUE]);
	}

	if ( yaail_txn_status(&FC.tx[FC_FAN_STALL_ID]) == YAAIL_DONE ) {
		FC.tx[FC_FAN_STALL_ID] = YAAIL_PENDING;
		if (!FC.ignoreFailureAtStart) {
			uint8_t data = FC.data[FC_FAN_STALL_VALUE];
			if ( (data & 0x01) == 0x00 ) {
				FC.status &= ~FAN_1_STALL;
			} else {
				FC.status |= FAN_1_STALL;
			}
			if ( (data & 0x02) == 0x00 ) {
				FC.status &= ~FAN_2_STALL;
			} else {
				FC.status |= FAN_2_STALL;
			}
		}
	}

	return FC.status;
}

#define implements_fan(nb) \
	void SetFan ## nb ## Power(uint8_t value) { \
		if ( value > 0 && value < 0x30) { \
			/*sets a minimum spin value of 18%, otherwise the fan won't spin*/ \
			value = 0x30; \
		} \
		yaail_txn_status_e s = yaail_txn_status(&FC.tx[FC_FAN ## nb ## _SP_ID]); \
		if ( s == YAAIL_SCHEDULED || s == YAAIL_PENDING ) { \
			return; \
		} \
		FC.data[FC_FAN## nb ## _SP_VALUE] = value; \
		yaail_write(&FC.tx[FC_FAN ## nb ##_SP_ID],EMC2302_1_I2C_ADDRESS,&FC.data[FC_FAN## nb ## _SP],2); \
	} \
	uint16_t GetFan ## nb ## RPM() { \
		return FC.RPM[nb -1]; \
	} \
	uint8_t GetFan ## nb ## Power() { \
		return FC.data[FC_FAN ## nb ##_SP_VALUE]; \
	}

implements_fan(1)
implements_fan(2)
