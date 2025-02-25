#include <hardware/gpio.h>
#include <optional>

#include <pico/stdlib.h>

#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

extern "C" {
#include <can2040.h>
}

static struct can2040 cbus;

std::optional<int64_t> print_uptime(absolute_time_t) {
	static int           i = 0;
	static can2040_stats stats;
	can2040_get_statistics(&cbus, &stats);
	Infof(
	    "It is %d, RX:%d TX:%d TX attempts: %d parse errors: %d",
	    i++,
	    stats.rx_total,
	    stats.tx_total,
	    stats.tx_attempt,
	    stats.parse_error
	);
	return std::nullopt;
}

struct CANEvent {
	enum class Type {
		RX    = CAN2040_NOTIFY_RX,
		TX    = CAN2040_NOTIFY_TX,
		ERROR = CAN2040_NOTIFY_ERROR,
	};

	inline static const char *TypeToString(Type t) {
		switch (t) {
		case Type::RX:
			return "CANEvent::Type::RX";
		case Type::TX:
			return "CANEvent::Type::TX";
		case Type::ERROR:
			return "CANEvent::Type::ERROR";
		}
		return "CANEvent::Type::<unknown>";
	}

	CANEvent::Type     Type;
	struct can2040_msg Message;
	CANEvent(){};

	CANEvent(uint32_t notify, struct can2040_msg *msg)
	    : Type{(enum Type)(notify)} {
		Message.id  = msg->id;
		Message.dlc = msg->dlc;
		for (size_t i = 0; i < std::min(msg->dlc, uint32_t(8)); ++i) {
			Message.data[i] = msg->data[i];
		}
	}
};

static Queue<CANEvent, 4, true> events;

static void
can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg) {
	events.EmplaceBlocking(notify, msg);
}

static void PIOx_IRQHandler(void) {
	can2040_pio_irq_handler(&cbus);
}

std::optional<int64_t> PrintEvents(absolute_time_t) {
	CANEvent evt;
	while (events.TryRemove(evt)) {
		Infof(
		    "Got can event %s ID:%x DATA:%x%x",
		    CANEvent::TypeToString(evt.Type),
		    evt.Message.id,
		    evt.Message.data32[0],
		    evt.Message.data32[1]
		);
	}
	return std::nullopt;
}

constexpr static uint TCAN_TX       = 12;
constexpr static uint TCAN_RX       = 13;
constexpr static uint TCAN_SHUTDOWN = 14;
constexpr static uint TCAN_STANDBY  = 15;

void canbus_setup(void) {
	// Setup canbus
	can2040_setup(&cbus, 0);
	can2040_callback_config(&cbus, can2040_cb);

	// Enable irqs
	irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
	irq_set_exclusive_handler(PIO0_IRQ_1, PIOx_IRQHandler);
	irq_set_priority(PIO0_IRQ_0, 1);
	irq_set_priority(PIO0_IRQ_1, 1);
	irq_set_enabled(PIO0_IRQ_0, true);
	irq_set_enabled(PIO0_IRQ_1, true);

	// Start canbus
	can2040_start(&cbus, SYS_CLK_HZ, 250000, TCAN_RX, TCAN_TX);
}

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();

	gpio_init(TCAN_SHUTDOWN);
	gpio_init(TCAN_STANDBY);
	gpio_init(TCAN_TX);
	gpio_set_dir(TCAN_TX, true);
	gpio_set_dir(TCAN_SHUTDOWN, true);
	gpio_set_dir(TCAN_STANDBY, true);
	gpio_put(TCAN_SHUTDOWN, 0);
	gpio_put(TCAN_STANDBY, 0);

	Scheduler::Schedule(100, 1000000, &print_uptime);
	Scheduler::Schedule(1, 0, &PrintEvents);

	canbus_setup();

	while (true) {
		Scheduler::Work();
	}
}
