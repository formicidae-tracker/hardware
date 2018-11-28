#include "FanControl.h"

#include <avr/pgmspace.h>

#include <yaail.h>
#include <emc2302.h>
#include "LEDs.h"
#include <arke-avr/systime.h>

#define FC_UPDATE_PERIOD 200
#define FC_IGNORE_ALERT_TIME 400


const PROGMEM uint16_t RPMLookUp[255]  = {0,61440,30720,20480,15360,12288,10240,8777,7680,6827,6144,5585,5120,4726,4389,4096,3840,3614,3413,3234,3072,2926,2793,2671,2560,2458,2363,2276,2194,2119,2048,1982,1920,1862,1807,1755,1707,1661,1617,1575,1536,1499,1463,1429,1396,1365,1336,1307,1280,1254,1229,1205,1182,1159,1138,1117,1097,1078,1059,1041,1024,1007,991,975,960,945,931,917,904,890,878,865,853,842,830,819,808,798,788,778,768,759,749,740,731,723,714,706,698,690,683,675,668,661,654,647,640,633,627,621,614,608,602,597,591,585,580,574,569,564,559,554,549,544,539,534,530,525,521,516,512,508,504,500,495,492,488,484,480,476,473,469,465,462,459,455,452,448,445,442,439,436,433,430,427,424,421,418,415,412,410,407,404,402,399,396,394,391,389,386,384,382,379,377,375,372,370,368,366,364,361,359,357,355,353,351,349,347,345,343,341,339,338,336,334,332,330,329,327,325,323,322,320,318,317,315,313,312,310,309,307,306,304,303,301,300,298,297,295,294,293,291,290,288,287,286,284,283,282,281,279,278,277,276,274,273,272,271,269,268,267,266,265,264,263,261,260,259,258,257,256,255,254,253,252,251,250,249,248,247,246,245,244,243,242};


typedef struct {
	yaail_txn_t setPoint[2],tachCount[2],stall;
	uint8_t setPointData[4],tachCountData[2],stallData[1];
} EMC2302Communication_t;


typedef struct {
	ArkeSystime_t last;
	FanControlStatus_e status;
	EMC2302Communication_t emc1;
#ifdef FAN_CONTROL_4FAN
	EMC2302Communication_t emc2;

	uint16_t RPM[4];
#else
	uint16_t RPM[2];
#endif
	bool ignoreFailureAtStart;
} FanControl_t;
FanControl_t FC;

#define init_emc_data(emc) do { \
		emc.setPointData[0] = EMC_FAN1_SET_POINT_REG; \
		emc.setPointData[1] = 0x00; \
		emc.setPointData[2] = EMC_FAN2_SET_POINT_REG; \
		emc.setPointData[3] = 0x00; \
		yaail_init_txn(&(emc.setPoint[0])); \
		yaail_init_txn(&(emc.setPoint[1])); \
		yaail_init_txn(&(emc.tachCount[0])); \
		yaail_init_txn(&(emc.tachCount[1])); \
		yaail_init_txn(&(emc.stall)); \
	}while(0)



void InitFanControl() {

	FC.last = 0;
	FC.RPM[0] = 0;
	FC.RPM[1] = 1;
	init_emc_data(FC.emc1);
#ifdef FAN_CONTROL_4FAN
	FC.RPM[2] = 0;
	FC.RPM[3] = 0;
	init_emc_data(FC.emc2);
#endif

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
	yaail_txn_t txn;


	for (uint8_t i = 0; i < NB_CONF; ++i ) {
		yaail_error_e err = yaail_write(&txn,EMC2302_1_I2C_ADDRESS,&registers_and_data[2*i],2);
		if ( err != YAAIL_NO_ERROR) {
			LEDErrorBlink(3);
		}
		yaail_spin_until_done(&txn);
#ifdef FAN_CONTROL_4FAN
		err = yaail_write(&txn,EMC2302_2_I2C_ADDRESS,&registers_and_data[2*i],2);
		if ( err != YAAIL_NO_ERROR) {
			LEDErrorBlink(3);
		}
		yaail_spin_until_done(&txn)
#endif
	}

	FC.ignoreFailureAtStart = true;
}

uint16_t tach_to_RPM(uint8_t highTachByte) {
	if ( highTachByte == 0xff ) {
		return 0;
	}
	return pgm_read_word(&(RPMLookUp[highTachByte]));
}

#define emc_push_get_info(emc,emcID) do {	  \
		emc.tachCountData[0] = EMC_FAN1_TACH_COUNT_REG_H; \
		emc.tachCountData[1] = EMC_FAN2_TACH_COUNT_REG_H; \
		emc.stallData[0] = EMC_FAN_STALL_REG; \
		yaail_write_and_read(&(emc.tachCount[0]),EMC2302_ ## emcID ## _I2C_ADDRESS,&(emc.tachCountData[0]),1,1); \
		yaail_write_and_read(&(emc.tachCount[1]),EMC2302_ ## emcID ## _I2C_ADDRESS,&(emc.tachCountData[1]),1,1); \
		yaail_write_and_read(&(emc.stall),EMC2302_ ## emcID ## _I2C_ADDRESS,&(emc.stallData[0]),1,1); \
	}while(0)

#define emc_pull_get_info(emc,fan1ID,fan2ID) do {	  \
		if ( yaail_txn_status(&(emc.tachCount[0])) == YAAIL_DONE ) { \
			emc.tachCount[0] = YAAIL_DONE+1; \
			FC.RPM[fan1ID] = tach_to_RPM(emc.tachCountData[0]); \
		} \
		if ( yaail_txn_status(&(emc.tachCount[1])) == YAAIL_DONE ) { \
			emc.tachCount[1] = YAAIL_DONE+1; \
			FC.RPM[fan2ID] = tach_to_RPM(emc.tachCountData[1]); \
		} \
		if ( yaail_txn_status(&(emc.stall)) == YAAIL_DONE ) { \
			emc.stall = YAAIL_DONE+1; \
			if (!FC.ignoreFailureAtStart) { \
				uint8_t data = emc.stallData[0]; \
				if ( (data & 0x01) == 0x00 ) { \
					FC.status &= ~(_BV(fan1ID)); \
				} else { \
					FC.status |= _BV(fan1ID); \
				} \
				if ( (data & 0x02) == 0x00 ) { \
					FC.status &= ~(_BV(fan2ID)); \
				} else { \
					FC.status |= _BV(fan2ID); \
				} \
			} \
		} \
	}while(0)



FanControlStatus_e ProcessFanControl() {

	ArkeSystime_t now = ArkeGetSystime();
	if ( (now - FC.last) >= FC_UPDATE_PERIOD ) {
		emc_push_get_info(FC.emc1,1);
#ifdef FAN_CONTROL_4FAN
		emc_push_get_info(FC.emc2,2);
#endif
		FC.last = now;
	}
	if ( FC.ignoreFailureAtStart && now >= FC_IGNORE_ALERT_TIME) {
		FC.ignoreFailureAtStart = false;
	}

	emc_pull_get_info(FC.emc1,0,1);
#ifdef FAN_CONTROL_4FA
	emc_pull_get_info(FC.emc2,2,3);
#endif

	return FC.status;
}

#define implements_fan(ID,emc,emcID)	  \
	void SetFan ## ID ## Power(uint8_t value) { \
		if ( value > 0 && value < 0x30) { \
			/*sets a minimum spin value of 18%, otherwise the fan won't spin*/ \
			value = 0x30; \
		} \
		yaail_txn_status_e s = yaail_txn_status(&(emc.setPoint[(ID-1)%2])); \
		if ( s == YAAIL_SCHEDULED || s == YAAIL_PENDING ) { \
			return; \
		} \
		emc.setPointData[((ID-1)%2)*2+1] = value; \
		yaail_write(&(emc.setPoint[(ID-1)%2]),EMC2302_ ## emcID ##_I2C_ADDRESS,&(emc.setPointData[2*((ID-1)%2)]),2); \
	} \
	uint16_t GetFan ## ID ## RPM() { \
		return FC.RPM[ID -1]; \
	} \
	uint8_t GetFan ## ID ## Power() { \
		return emc.setPointData[2*((ID-1)%2)+1]; \
	}

implements_fan(1,FC.emc1,1)
implements_fan(2,FC.emc1,1)
#ifdef FAN_CONTROL_4FAN
implements_fan(3,FC.emc2,2)
implements_fan(4,FC.emc2,2)
#endif
