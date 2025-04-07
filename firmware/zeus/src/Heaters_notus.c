#include "Heaters.h"
#include "arke-avr.h"
#include "arke-avr/systime.h"
#include "txn.h"
#include <stdbool.h>

struct NotusHeaters_t {
	ArkeSystime_t lastUpdate;
	uint8_t       nextCommand;
	bool          hasCommand;
	yaacl_txn_t   txn;
};

static struct NotusHeaters_t notus;

#define MIN_NOTUS_UPDATE_PERIOD_MS 200

void InitHeaters() {
	notus.lastUpdate = ArkeGetSystime() - MIN_NOTUS_UPDATE_PERIOD_MS;
	notus.hasCommand = false;
	yaacl_init_txn(&notus.txn);
}

void ProcessHeaters(ArkeSystime_t now) {
	if (notus.hasCommand == false ||
	    (now - notus.lastUpdate) < MIN_NOTUS_UPDATE_PERIOD_MS ||
	    yaacl_txn_status(&notus.txn) == YAACL_TXN_PENDING) {
		return;
	}

	ArkeNotusSetPoint sp;
	sp.Power = notus.nextCommand;
	ArkeSendNotusSetPoint(&notus.txn, false, &sp);
	notus.hasCommand = false;
}

void HeatersSetPower(uint8_t power) {
	notus.nextCommand = power;
	notus.hasCommand  = true;
}
