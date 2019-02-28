#include "FanControl.h"

#include <avr/pgmspace.h>

#include <yaail.h>
#include <emc2302.h>
#include "LEDs.h"
#include <arke-avr/systime.h>

#define FC_UPDATE_PERIOD 100
#define FC_IGNORE_ALERT_TIME 2000
#define FAN_AGING_THRESHOLD(power) ( 4 * power)


const PROGMEM uint16_t RPMLookUp[255]  = {0,61440,30720,20480,15360,12288,10240,8777,7680,6827,6144,5585,5120,4726,4389,4096,3840,3614,3413,3234,3072,2926,2793,2671,2560,2458,2363,2276,2194,2119,2048,1982,1920,1862,1807,1755,1707,1661,1617,1575,1536,1499,1463,1429,1396,1365,1336,1307,1280,1254,1229,1205,1182,1159,1138,1117,1097,1078,1059,1041,1024,1007,991,975,960,945,931,917,904,890,878,865,853,842,830,819,808,798,788,778,768,759,749,740,731,723,714,706,698,690,683,675,668,661,654,647,640,633,627,621,614,608,602,597,591,585,580,574,569,564,559,554,549,544,539,534,530,525,521,516,512,508,504,500,495,492,488,484,480,476,473,469,465,462,459,455,452,448,445,442,439,436,433,430,427,424,421,418,415,412,410,407,404,402,399,396,394,391,389,386,384,382,379,377,375,372,370,368,366,364,361,359,357,355,353,351,349,347,345,343,341,339,338,336,334,332,330,329,327,325,323,322,320,318,317,315,313,312,310,309,307,306,304,303,301,300,298,297,295,294,293,291,290,288,287,286,284,283,282,281,279,278,277,276,274,273,272,271,269,268,267,266,265,264,263,261,260,259,258,257,256,255,254,253,252,251,250,249,248,247,246,245,244,243,242};


uint16_t tach_to_RPM(uint8_t highTachByte,FanRPMRange_e mult) {
	if ( highTachByte == 0xff ) {
		return 0;
	}
	return mult * pgm_read_word(&(RPMLookUp[highTachByte]));
}


typedef struct {
	uint8_t address;
	yaail_txn_t setPoint[2],tachCount[2];
	uint8_t setPointData[4],tachCountData[2];
	bool newSetPoint[2];
	uint8_t targetSetPoint[2];
	uint16_t RPM[2];
	FanRPMRange_e fanMult[2];
	ArkeSystime_t setPointOnTime[2];
} EMC2302Communication_t;


void InitEMC(EMC2302Communication_t *emc, uint8_t address, uint8_t fan1Mult , uint8_t fan2Mult ) {
	emc->address = address;
	emc->setPointData[0] = EMC_FAN1_SET_POINT_REG;
	emc->setPointData[1] = 0x00;
	emc->setPointData[2] = EMC_FAN2_SET_POINT_REG;
	emc->setPointData[3] = 0x00;
	yaail_init_txn(&(emc->setPoint[0]));
	yaail_init_txn(&(emc->setPoint[1]));
	yaail_init_txn(&(emc->tachCount[0]));
	yaail_init_txn(&(emc->tachCount[1]));
	emc->RPM[0] = 0;
	emc->RPM[1] = 0;

#define NB_CONF 10

	uint8_t registers_and_data[2*NB_CONF] = {EMC_FAN1_SET_POINT_REG,0x00,
	                                         EMC_FAN2_SET_POINT_REG,0x00,
	                                         EMC_FAN1_CONF1_REG, EMC_DIS_ALGO | EMC_RANGE_500 | EMC_EDGES_3 | EMC_UPDATE_200MS,
	                                         EMC_FAN2_CONF1_REG, EMC_DIS_ALGO | EMC_RANGE_500 | EMC_EDGES_3 | EMC_UPDATE_200MS,
	                                         EMC_FAN1_CONF2_REG, EMC_ENABLE_RAMP_RATE_CONTROL | EMC_ENABLE_GLITCH_FILTER | EMC_BASIC_DERIVATIVE | EMC_ERROR_RANGE_50RPM,
	                                         EMC_FAN2_CONF2_REG, EMC_ENABLE_RAMP_RATE_CONTROL | EMC_ENABLE_GLITCH_FILTER | EMC_BASIC_DERIVATIVE | EMC_ERROR_RANGE_50RPM,
	                                         EMC_FAN1_SPIN_UP_REG,EMC_DRIVE_FAIL_COUNT_16 | EMC_ENABLE_KICK | EMC_SPIN_TIME_1000MS | EMC_SPIN_LVL_65,
	                                         EMC_FAN2_SPIN_UP_REG,EMC_DRIVE_FAIL_COUNT_16 | EMC_ENABLE_KICK | EMC_SPIN_TIME_1000MS | EMC_SPIN_LVL_65,
	                                         EMC_FAN1_VALID_TACH_REG,0xfd,
	                                         EMC_FAN2_VALID_TACH_REG,0xfd
	};

	if (fan1Mult > 1 || fan1Mult == 0) {
		fan1Mult = 2;
		registers_and_data[5] = EMC_DIS_ALGO | EMC_RANGE_1000 | EMC_EDGES_3 | EMC_UPDATE_200MS;
	}
	emc->fanMult[0] = fan1Mult;

	if (fan2Mult > 1 || fan2Mult == 0) {
		fan2Mult = 2;
		registers_and_data[7] = EMC_DIS_ALGO | EMC_RANGE_1000 | EMC_EDGES_3 | EMC_UPDATE_200MS;
	}
	emc->fanMult[1] = fan2Mult;

	yaail_txn_t txn;

	for (uint8_t i = 0; i < NB_CONF; ++i ) {
		yaail_error_e err = yaail_write(&txn,emc->address,&registers_and_data[2*i],2);
		if ( err != YAAIL_NO_ERROR) {
			LEDErrorBlink(3);
		}
		yaail_spin_until_done(&txn);
	}


}

void EMCSendValue(EMC2302Communication_t * emc, ArkeSystime_t  now) {
	for (uint8_t i = 0; i <2; ++i ) {
		if (emc->newSetPoint[i] == false) {
			continue;
		}
		yaail_txn_status_e s = yaail_txn_status(&(emc->setPoint[i]));
		if ( s == YAAIL_SCHEDULED || s == YAAIL_PENDING ) {
			continue;
		}
		uint8_t current = emc->setPointData[2*i+1];
		uint8_t target = emc->targetSetPoint[i];
		if ( current == 0 && target > 0 ) {
			emc->setPointOnTime[i] = now;
		}
		emc->setPointData[2*i+1] = target;
		yaail_write(&(emc->setPoint[i]),emc->address,&(emc->setPointData[2*i]),2);
	}
}

void EMCPushGetInfo(EMC2302Communication_t * emc) {
	emc->tachCountData[0] = EMC_FAN1_TACH_COUNT_REG_H;
	emc->tachCountData[1] = EMC_FAN2_TACH_COUNT_REG_H;
	yaail_write_and_read(&(emc->tachCount[0]),emc->address,&(emc->tachCountData[0]),1,1);
	yaail_write_and_read(&(emc->tachCount[1]),emc->address,&(emc->tachCountData[1]),1,1);
}


void EMCUpdateFanStatus(EMC2302Communication_t * emc,uint8_t index,ArkeSystime_t now, uint16_t rpm) {
	uint8_t target = emc->setPointData[2*index+1];
	ArkeSystime_t start = emc->setPointOnTime[index];
	if ( target == 0 || (now-start) < FC_IGNORE_ALERT_TIME ) {
		emc->RPM[index]  = rpm;
		return;
	}
	if ( rpm == 0 ) { \
		emc->RPM[index] = ARKE_FAN_STALL_ALERT | rpm;
		return;
	}
	if ( rpm < FAN_AGING_THRESHOLD(target) ) {
		emc->RPM[index] = ARKE_FAN_AGING_ALERT | rpm;
		return;
	}
	emc->RPM[index] = rpm;
}

bool EMCPullInfo(EMC2302Communication_t * emc, ArkeSystime_t now) {
	bool hasNew = false;
	for (uint8_t i = 0; i < 2; ++i) {
		if ( yaail_txn_status(&(emc->tachCount[i])) == YAAIL_DONE ) {
			emc->tachCount[i] = YAAIL_DONE+1; // we avoid looping
			EMCUpdateFanStatus(emc,i,now,tach_to_RPM(emc->tachCountData[i],emc->fanMult[i]));
			hasNew = true;
		}
	}
	return hasNew;
}

void EMCSetFanPower(EMC2302Communication_t * emc, uint8_t index, uint8_t value) {
	index = index % 2;
	if ( value > 0 && value < 0x30) {
		//sets a minimum spin value of 18%, otherwise the fan won't spin
		value = 0x30;
	}
	emc->targetSetPoint[index] = value;
	emc->newSetPoint[index] = true;
}

uint16_t EMCGetFanRPM(EMC2302Communication_t * emc, uint8_t index) {
	return emc->RPM[index%2];
}

uint8_t EMCGetFanPower(EMC2302Communication_t * emc, uint8_t index) {
	return emc->targetSetPoint[index%2];
}

#ifdef FAN_CONTROL_4FAN
#define NB_EMC 2
#else
#define NB_EMC 1
#endif

typedef struct {
	ArkeSystime_t last;
	EMC2302Communication_t emc[NB_EMC];
} FanControl_t;
FanControl_t FC;




#ifdef FAN_CONTROL_4FAN
void InitFanControl(FanRPMRange_e fan1, FanRPMRange_e fan2, FanRPMRange_e fan3, FanRPMRange_e fan4) {
#else
void InitFanControl(FanRPMRange_e fan1, FanRPMRange_e fan2) {
#endif
	FC.last = 0;
	InitEMC(&FC.emc[0],EMC2302_1_I2C_ADDRESS,fan1,fan2);
#ifdef FAN_CONTROL_4FAN
	InitEMC(&FC.emc[1],EMC2302_2_I2C_ADDRESS,fan3,fan4);
#endif
}


bool ProcessFanControl() {
	bool hasNew = false;
	ArkeSystime_t now = ArkeGetSystime();

	for (uint8_t i = 0; i <NB_EMC; ++i ) {
		if (EMCPullInfo(&(FC.emc[i]),now) == true ){
			hasNew = true;
		}
		EMCSendValue(&(FC.emc[i]),now);
	}

	if ( (now - FC.last) < FC_UPDATE_PERIOD) {
		return hasNew;
	}
	FC.last = now;

	for (uint8_t i = 0; i <NB_EMC; ++i ) {
		EMCPushGetInfo(&(FC.emc[i]));
	}

	return hasNew;
}

#define implements_fan(ID,emcIdx,fanIdx) \
	void SetFan ## ID ## Power(uint8_t value) { \
		EMCSetFanPower(&(FC.emc[emcIdx]),fanIdx,value); \
	} \
	uint16_t GetFan ## ID ## RPM() { \
		return EMCGetFanRPM(&(FC.emc[emcIdx]),fanIdx); \
	} \
	uint8_t GetFan ## ID ## Power() {\
		return EMCGetFanPower(&(FC.emc[emcIdx]),fanIdx); \
	}


 implements_fan(1,0,0)
 implements_fan(2,0,1)
#ifdef FAN_CONTROL_4FAN
 implements_fan(3,1,0)
 implements_fan(4,1,1)
#endif
