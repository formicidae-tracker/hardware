#include "canbus.h"
#include "canbus.pio.h"

#include <array>
#include <cstdint>
#include <exception>
#include <hardware/irq.h>
#include <hardware/pio.h>
#include <hardware/platform_defs.h>
#include <pico/platform/panic.h>
#include <utility>

#include <utils/Queue.hpp>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

struct TxMessage {
	TxMessage(){};

	TxMessage(struct canbus_msg *msg) {
		// todo: fill data with CRC and stuffed data;
	}

	uint8_t  size;
	uint32_t data[5];
};

static struct canbus *instances[NUM_PIOS];

extern irq_handler_t canbus_irq_0_handlers[NUM_PIOS];

constexpr static uint SIGNAL_RX_DATA_FLAG =
    (1u << (pis_sm0_rx_fifo_not_empty + CANBUS_RX_SM));
constexpr static uint SIGNAL_SOF_FLAG = (1u << IRQ_SIGNAL_SOF);
constexpr static uint SIGNAL_EOF_FLAG = (1u << IRQ_SIGNAL_EOF);
constexpr static uint SIGNAL_TX_PENDING_FLAG = (1u << IRQ_SIGNAL_TX_PENDING);

struct canbus {
	enum class State {
		IDLE,
		RX,
		TX,
	};

	canbus(struct canbus_config *config)
	    : pio{pio_get_instance(config->pio_num)}
	    , callback(config->callback) {
		if (config->pio_num >= NUM_PIOS) {
			panic(
			    "invalid PIO number %d, must be <%d",
			    config->pio_num,
			    NUM_PIOS
			);
		}
		instances[config->pio_num] = this;
		auto irq_num               = pio_get_irq_num(pio, 0);
		irq_set_exclusive_handler(
		    irq_num,
		    canbus_irq_0_handlers[config->pio_num]
		);
		irq_set_priority(irq_num, 1);
		irq_num = pio_get_irq_num(pio, 1);

		// high priority interrupt
		pio_set_irq0_source_mask_enabled(
		    pio,
		    SIGNAL_RX_DATA_FLAG | SIGNAL_SOF_FLAG | SIGNAL_EOF_FLAG |
		        SIGNAL_TX_PENDING_FLAG,
		    true
		);
	}

	void irq() {
		uint irq_flags = pio->ints0;
		while (likely(irq_flags & SIGNAL_RX_DATA_FLAG)) {
			process_rx(pio->rxf[CANBUS_RX_SM]);
			irq_flags = pio->ints0;
			if (likely(!irq_flags)) {
				return;
			}
		}

		if (irq_flags & SIGNAL_SOF_FLAG) {
			got_sof();
		}
		if (irq_flags & SIGNAL_EOF_FLAG) {
			got_eof();
		}
		if (irq_flags & SIGNAL_TX_PENDING_FLAG) {
			got_tx_pending();
		}
	}

	void process_rx(uint32_t data) {
		// TODO we should process incoming message or outgoing message, check
		// for collision, check for CRC, schedule PIO CRC check, check for
		// ACK/NACK, and then emit events RX TX or failed.
	}

	void got_sof() {
		if (state == State::IDLE) {
			state = State::RX;
		}
	}

	void got_eof() {
		// we can return to idle now
		wrap_ensure_disabled(pio, CANBUS_ACK_SM, {
			canbus_ack_program_restart(pio);
			canbus_ack_program_reset_counters(pio);
		});

		wrap_ensure_disabled(pio, CANBUS_TX_SM, {
			// clear all fifos, we may have remaining data from a failed fifo.
			pio_sm_clear_fifos(pio, CANBUS_TX_SM);
			// we clear any endless loop if we were transmitting.
			canbus_tx_program_reset(pio);
		});
		state = State::IDLE;
		if (tx_queue.Empty() == false) {
			got_tx_pending();
		}
	}

	void got_tx_pending() {
		if (state != State::IDLE) {
			return;
		}
		// TODO: fill out FIFO with the data, do not remove it.
	}

	void report_tx_pending() {
		pio->irq_force |= SIGNAL_TX_PENDING_FLAG;
	}

	void Start() {
		irq_set_enabled(pio_get_irq_num(pio, 0), true);
		pio_set_sm_mask_enabled(pio, 0x000f, true);
	}

	void Task() {}

	void Stop() {}

	PIO                 pio;
	canbus_rx_cb        callback;
	struct canbus_stats stats = {
	    .total_rx    = 0,
	    .total_tx    = 0,
	    .tx_attempts = 0,
	    .rx_errors   = 0,
	};

	Queue<TxMessage, 4, true> tx_queue;

	State state = State::IDLE;
	};

#if NUM_PIOS >= 1
static void canbus_handler_pio_0_irq_0() {
	instances[0]->irq();
}

#endif

#if NUM_PIOS >= 2
static void canbus_handler_pio_1_irq_0() {
	instances[1]->irq();
}

#endif

#if NUM_PIOS >= 3
static void canbus_handler_pio_2_irq_0() {
	instances[2]->irq();
}

#endif

irq_handler_t canbus_irq_0_handlers[NUM_PIOS] = {
    canbus_handler_pio_0_irq_0,
#if NUM_PIOS >= 2
    canbus_handler_pio_1_irq_0,
#endif
#if NUM_PIOS >= 3
    canbus_handler_pio_2_irq_0,
#endif
};

extern "C" {
struct canbus *canbus_setup(struct canbus_config *config) {
	return new struct canbus(config);
}

void canbus_start(struct canbus *b) {
	b->Start();
}

void canbus_task(struct canbus *b) {
	b->Task();
}

void canbus_stop(struct canbus *b) {
	b->Stop();
}

void canbus_get_statistics(struct canbus *b, struct canbus_stats *stats) {
	*stats = b->stats;
}

int canbus_queue_transmit(struct canbus *b, struct canbus_msg *msg) {
	TxMessage tx{msg};
	if (b->tx_queue.TryAdd(tx) == false) {
		return CANBUS_ERROR_TX_QUEUE_FULL;
	}
	return 0;
}

int canbus_can_transmit(struct canbus *b) {
	return b->tx_queue.Full() == false;
}
}
