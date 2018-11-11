#include "FanControl.h"

#include <avr/pgmspace.h>

#include <yaail.h>
#include <emc2302.h>
#include "LEDs.h"
#include "Systime.h"
#include <yaacl.h>

const PROGMEM uint16_t RPMLookUp[255]  = {0,61440,30720,20480,15360,12288,10240,8777,7680,6827,6144,5585,5120,4726,4389,4096,3840,3614,3413,3234,3072,2926,2793,2671,2560,2458,2363,2276,2194,2119,2048,1982,1920,1862,1807,1755,1707,1661,1617,1575,1536,1499,1463,1429,1396,1365,1336,1307,1280,1254,1229,1205,1182,1159,1138,1117,1097,1078,1059,1041,1024,1007,991,975,960,945,931,917,904,890,878,865,853,842,830,819,808,798,788,778,768,759,749,740,731,723,714,706,698,690,683,675,668,661,654,647,640,633,627,621,614,608,602,597,591,585,580,574,569,564,559,554,549,544,539,534,530,525,521,516,512,508,504,500,495,492,488,484,480,476,473,469,465,462,459,455,452,448,445,442,439,436,433,430,427,424,421,418,415,412,410,407,404,402,399,396,394,391,389,386,384,382,379,377,375,372,370,368,366,364,361,359,357,355,353,351,349,347,345,343,341,339,338,336,334,332,330,329,327,325,323,322,320,318,317,315,313,312,310,309,307,306,304,303,301,300,298,297,295,294,293,291,290,288,287,286,284,283,282,281,279,278,277,276,274,273,272,271,269,268,267,266,265,264,263,261,260,259,258,257,256,255,254,253,252,251,250,249,248,247,246,245,244,243,242};

typedef enum {
	NO_ACTION = 0,
	SET_F1 = (1<<0),
	SET_F2 = (1<<1),
	GET_F1 = (1<<2),
	GET_F2 = (1<<3),
	GET_STALL = (1 << 4)
} FCAction_e;

#define FC_UPDATE_PERIOD 200


typedef struct {
	Systime_t last;
	FCAction_e plannedActions;
	FCAction_e currentAction;
	FanControlStatus_e status;
	uint8_t setPoints[2];
	uint16_t RPM[2];
	uint8_t data[2];
	yaacl_txn_t tx;
} FanControl_t;
FanControl_t FC;

void InitFanControl() {
	yaail_init(YAAIL_100);

	uint8_t registers_and_data[2*6] = {EMC_FAN1_SET_POINT_REG,0x00,
	                                   EMC_FAN2_SET_POINT_REG,0x00,
	                                   EMC_FAN1_CONF1_REG, EMC_DIS_ALGO | EMC_RANGE_500 | EMC_EDGES_3 | EMC_UPDATE_200MS,
	                                   EMC_FAN2_CONF1_REG, EMC_DIS_ALGO | EMC_RANGE_500 | EMC_EDGES_3 | EMC_UPDATE_200MS,
	                                   EMC_FAN1_CONF2_REG, 0x68,
	                                   EMC_FAN2_CONF2_REG, 0x68};
	FC.last = 0;
	FC.plannedActions = NO_ACTION;
	FC.setPoints[0] = 0;
	FC.setPoints[1] = 0;
	FC.RPM[0] = 0;
	FC.RPM[1] = 1;
	FC.status = FAN_OK;
	for (uint8_t i = 0; i < 6; ++i ) {
		yaail_status_e err = yaail_write_and_spin(EMC2302_1_I2C_ADDRESS,&registers_and_data[2*i],2);
		if ( err != YAAIL_DONE) {
			LEDErrorBlink(3);
		}
		yaail_rearm();
	}

	yaacl_init_txn(&FC.tx);
	yaacl_make_std_idt(FC.tx.ID,0,0);
	FC.tx.data = (uint8_t*)&FC.RPM[0];
	FC.tx.length = 2;
}

uint16_t tach_to_RPM(uint8_t highTachByte) {
	if ( highTachByte == 0xff ) {
		return 0;
	}
	return pgm_read_word(&(RPMLookUp[highTachByte]));
}



FanControlStatus_e ProcessFanControl() {
	yaacl_txn_status(&FC.tx);
	Systime_t now = GetSystime();
	if ( (now - FC.last) >= FC_UPDATE_PERIOD ) {
		FC.plannedActions |= GET_F1 | GET_F2 | GET_STALL;
		FC.last = now;
	}

	switch(yaail_poll()) {
	case YAAIL_BUSY:
		return FC.status;
	case YAAIL_START_ERROR:
		yaail_rearm();
		LEDErrorBlink(4);
		return FC.status;
	case YAAIL_DONE:
	case YAAIL_ACK_ERROR:

		switch(FC.currentAction) {
		case GET_F1:
			FC.RPM[0] = tach_to_RPM(FC.data[0]);
			yaacl_send(&FC.tx);
			break;
		case GET_F2:
			FC.RPM[1] = tach_to_RPM(FC.data[0]);
			break;
		case GET_STALL:
			if( (FC.data[0] & 0x01) != 0x00 ) {
				FC.status |= FAN_1_STALL;
			} else {
				FC.status &= ~FAN_1_STALL;
			}
			if( (FC.data[0] & 0x02) != 0x00 ) {
				FC.status |= FAN_2_STALL;
			} else {
				FC.status &= ~FAN_2_STALL;
			}
			break;
		default:
			break;
		}


		yaail_rearm();
	default:
		break;
	}


	if(FC.plannedActions == NO_ACTION) {
		return FC.status;
	}

	if ( (FC.plannedActions & SET_F1) != 0x00) {
		FC.data[0] = EMC_FAN1_SET_POINT_REG;
		FC.data[1] = FC.setPoints[0];
		yaail_write(EMC2302_1_I2C_ADDRESS,FC.data,2);
		FC.currentAction = SET_F1;
		FC.plannedActions &= ~SET_F1;
		return FC.status;
	}

	if ( (FC.plannedActions & SET_F2) != 0x00) {
		FC.data[0] = EMC_FAN2_SET_POINT_REG;
		FC.data[1] = FC.setPoints[1];
		yaail_write(EMC2302_1_I2C_ADDRESS,FC.data,2);
		FC.currentAction = SET_F2;
		FC.plannedActions &= ~SET_F2;
		return FC.status;
	}

	if ( (FC.plannedActions & GET_F1) != 0x00) {
		FC.data[0] = EMC_FAN1_TACH_COUNT_REG_H;
		yaail_write_and_read(EMC2302_1_I2C_ADDRESS,FC.data,1,1);
		FC.currentAction = GET_F1;
		FC.plannedActions &= ~GET_F1;
		return FC.status;
	}

	if ( (FC.plannedActions & GET_F2) != 0x00) {
		FC.data[0] = EMC_FAN2_TACH_COUNT_REG_H;
		yaail_write_and_read(EMC2302_1_I2C_ADDRESS,FC.data,1,1);
		FC.currentAction = GET_F2;
		FC.plannedActions &= ~GET_F2;
		return FC.status;
	}

	if ( (FC.plannedActions & GET_STALL) != 0x00) {
		FC.data[0] = EMC_FAN_STALL_REG;
		yaail_write_and_read(EMC2302_1_I2C_ADDRESS,FC.data,1,1);
		FC.currentAction = GET_STALL;
		FC.plannedActions &= ~GET_STALL;
		return FC.status;
	}


	return FC.status;
}

void SetFan1Power(uint8_t value) {
	FC.setPoints[0] = value;
	FC.plannedActions |= SET_F1;
}
void SetFan2Power(uint8_t value) {
	FC.setPoints[1] = value;
	FC.plannedActions |= SET_F2;
}

uint16_t GetFan1RPM() {
	return FC.RPM[0];
}
uint16_t GetFan2RPM() {
	return FC.RPM[1];
}
