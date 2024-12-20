#pragma once
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>

struct canbus_msg {
	uint32_t idt;
	uint8_t  dlc;

	union {
		uint8_t  data[8];
		uint32_t data32[2];
	};
};

enum {
	CANBUS_IDT_RTR = 1 << 30,
	CANBUS_ID_EFF  = 1 << 31,
};

enum {
	CANBUS_NOTIFY_RX    = 1,
	CANBUS_NOTIFY_TX    = 2,
	CANBUS_NOTIFY_ERROR = 3,
};

enum { CANBUS_ERROR_TX_QUEUE_FULL = 1 };

struct canbus;

struct canbus_stats {
	uint32_t total_rx;
	uint32_t total_tx;
	uint32_t tx_attempts;
	uint32_t rx_errors;
};

typedef void (*canbus_rx_cb
)(struct canbus *b, uint8_t event, struct canbus_msg *msg);

struct canbus_config {
	uint32_t     rx_pin, tx_pin;
	uint32_t     baudrate;
	uint32_t     pio_num;
	canbus_rx_cb callback;
};

struct canbus *canbus_setup(canbus_config *config);
void canbus_start(struct canbus *b);
void           canbus_task(struct canbus *b);
void canbus_stop(struct canbus *b);
void canbus_get_statistics(struct canbus *b, struct canbus_stats *stats);
int  canbus_queue_transmit(struct canbus *b, struct canbus_msg *msg);
int  canbus_can_transmit(struct canbus *b);

#ifdef __cplusplus
}
#endif //__cplusplus
