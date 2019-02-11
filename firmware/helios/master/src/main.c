#include <avr/io.h>

#include "Systime.h"
#include "ModuleManager.h"

#include <arke-avr.h>

#include <string.h>

#define DEAD_TIME (int16_t)10

#define min(a,b) ( (a) < (b) ? (a) : (b) )
#define max(a,b) ( (a) > (b) ? (a) : (b) )
#define clamp(value,low,high) (min(max(value,low),high))

struct HeliosMaster_t {
	union {
		ArkeHeliosSetPoint setPoint;
		uint8_t bytes[2];
	} inBuffer;

	yaacl_txn_t tx;

	bool pulseMode;
	int16_t pulseValue;
	int16_t pulseIncrement;
	ArkeSystime_t lastPulse;
};
struct HeliosMaster_t HM;

#define MY_MASK ( (ARKE_NODE_CLASS_MASK & (~0x03)) | ARKE_SUBID_MASK)

implements_ArkeSoftwareReset()

void ProcessIncoming() {
	uint8_t  dlc = 0;
	yaacl_idt_t idt = ArkeProcess(&dlc);
	bool rtr = yaacl_idt_test_rtrbit(idt);
	ArkeMessageClass a = (idt & 0x1f8) >> 3;
	switch (a) {
	case ARKE_HELIOS_SET_POINT:
		if (rtr && yaacl_txn_status(&HM.tx) != YAACL_TXN_PENDING ) {
			ArkeSendHeliosSetPoint(&HM.tx,false,&(HM.inBuffer.setPoint));
			return;
		}
		SendToModule(HM.inBuffer.setPoint.Visible,HM.inBuffer.setPoint.UV);
		break;
	case ARKE_HELIOS_PULSE_MODE:
		HM.pulseMode = !HM.pulseMode;
		if (HM.pulseMode) {
			HM.lastPulse = ArkeGetSystime();
			HM.pulseValue = HM.inBuffer.setPoint.Visible;
			HM.pulseIncrement = 1;
		} else {
			SendToModule(HM.inBuffer.setPoint.Visible,HM.inBuffer.setPoint.UV);
		}
		break;
	default:
		return;
	}
}

int main() {
	HM.pulseMode = false;
	HM.pulseValue = 0;
	HM.pulseIncrement = 1;
	InitModuleManager();

	HM.inBuffer.setPoint.Visible = 0;
	HM.inBuffer.setPoint.UV = 0;
	yaacl_init_txn(&HM.tx);
	InitArke(&(HM.inBuffer.bytes[0]),sizeof(HM.inBuffer));


	SendToModule(HM.inBuffer.setPoint.Visible,HM.inBuffer.setPoint.UV);

	while(1) {
		ProcessModuleManager();
		ProcessIncoming();
		if(!HM.pulseMode) {
			continue;
		}
		ArkeSystime_t now = ArkeGetSystime();
		if ( (now - HM.lastPulse) < 10 ) {
			continue;
		}
		HM.lastPulse = now;
		if (HM.pulseValue >= (int16_t)255 + DEAD_TIME) {
			HM.pulseIncrement = -1;
		} else if (HM.pulseValue <= -DEAD_TIME) {
			HM.pulseIncrement = +1;
		}
		HM.pulseValue += HM.pulseIncrement;
		uint8_t toSend = clamp(HM.pulseValue,(int16_t)0,(int16_t)255);
		SendToModule(toSend,toSend);

	}

	return 0;
}
