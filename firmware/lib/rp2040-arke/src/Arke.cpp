#include "Arke.hpp"

#include "hardware/irq.h"
#include "hardware/pio.h"

#include <algorithm>
#include <cstdint>
#include <hardware/platform_defs.h>
#include <hardware/regs/intctrl.h>
#include <optional>

#include <utils/Log.hpp>

#include <utils/Queue.hpp>
#include <utils/Scheduler.hpp>

class Arke {
public:
	void Init(ArkeConfig &&config);
	void Work();
};

static Arke arke;

void ArkeInit(ArkeConfig &&config) {
	arke.Init(std::move(config));
}

static void arke_can2040_irq_handler(void) {
	Infof("coucou from IRQ");
}

static void
arke_can2040_cb(struct can2040 *bus, uint32_t notify, struct can2040_msg *msg) {

}

std::optional<int64_t> arkeWork(absolute_time_t now) {
	arke.Work();
	return std::nullopt;
}

std::optional<int64_t> arkePong(absolute_time_t now) {
	return std::nullopt;
}

void Arke::Init(ArkeConfig &&config) {
	Scheduler::Schedule(0, 0, arkeWork);
	Scheduler::Schedule(10, 1000000, arkePong);
}

void Arke::Work() {}
