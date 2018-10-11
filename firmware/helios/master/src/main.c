#include <avr/io.h>

#include "Systime.h"
#include "ModuleManager.h"

#include <arke-avr.h>

#include <string.h>

#define DEAD_TIME 30

struct MasterData {
	yaacl_txn_t rx[2];
	yaacl_txn_t tx;
	uint8_t dataBuffer[8];
	ArkeHeliosSetPoint setPoint;
};
struct MasterData app;

#define MY_MASK ( (ARKE_NODE_CLASS_MASK & (~0x03)) | ARKE_SUBID_MASK)

implements_ArkeSoftwareReset()

void ProcessIncoming() {

	for ( uint8_t i = 0 ; i < 2; ++i) {
		yaacl_txn_status_e s = yaacl_txn_status(&(app.rx[i]));
		if ( s == YAACL_TXN_PENDING ) {
			continue;
		}
		uint8_t length = app.rx[i].length;
		app.rx[i].length = 8;
		yaacl_listen(&(app.rx[i]));

		if ( ((app.rx[i].ID & ARKE_MESSAGE_TYPE_MASK) >> 9) == ARKE_HEARTBEAT ) {
			//discard
			continue;
		}

		uint8_t mID = (app.rx[i].ID & ARKE_NODE_CLASS_MASK) >> 3;
		bool rtr = yaacl_idt_test_rtrbit(app.rx[i].ID);

		//we are sure its either a broadcast or
		if (mID != ARKE_HELIOS_SET_POINT ) {
			continue;
		}
		if (rtr) {
			ArkeSendHeliosSetPoint(&(app.tx),false,false,ARKE_MY_SUBID,&(app.setPoint));
			continue;
		}

		if (length != 2) {
			continue;
		}

		memcpy(&(app.setPoint),app.rx[i].data,2);
		SendToModule(app.setPoint.Visible, app.setPoint.UV);

	}
}

int main() {
	InitArke();
	app.setPoint.Visible = 0;
	app.setPoint.UV = 0;
	yaacl_init_txn(&(app.rx[0]));
	yaacl_init_txn(&(app.rx[1]));
	yaacl_init_txn(&(app.tx));
	yaacl_make_std_idt(app.rx[0].ID,ARKE_MY_CLASS << 3 | ARKE_MY_SUBID,0);

	yaacl_make_std_idt(app.rx[1].ID,ARKE_MY_CLASS << 3,0);
	SendToModule(app.setPoint.Visible,app.setPoint.UV);

	for ( uint8_t i = 0 ; i < 2; ++i) {
		app.rx[i].length = 8;
		app.rx[i].data = &(app.dataBuffer[0]);
		yaacl_make_std_mask(app.rx[i].mask,MY_MASK,0,1);
		yaacl_listen(&(app.rx[i]));

	}

	InitModuleManager();

	while(1) {
		ProcessModuleManager();
		ArkeProcess();
		ProcessIncoming();
		yaacl_txn_status(&(app.tx));

		// implements

	}

	return 0;
}
