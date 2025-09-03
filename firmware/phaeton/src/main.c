#include <stdbool.h>
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define PIN_DEBUG _BV(1)
#define PIN_IN    _BV(7)

struct UARTIncoming {
	char    Buffer[3];
	uint8_t next;
};

enum State {
	IDLE,
	DEBOUNCE,
	PULSE,
	REST,
	UART_RX,
};

#define US_TO_TICKS(a)      (((a)*5) / 4)
#define DEBOUNCE_TIME_ticks 75    // US_TO_TICKS(60)
#define MAX_PULSE_ticks     5625  // US_TO_TICKS(4500)
#define MIN_PERIOD_ticks    39062 // US_TO_TICKS(31250)

#define UART_RX_TIME_ticks  2500  // US_TO_TICKS(2000)

#define WORD_HIGH(a) (uint8_t)(((a) >> 8) & 0xff)
#define WORD_LOW(a)  (uint8_t)((a)&0xff)

// Inits the timer
inline static void initTimer() {
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;

	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm | TCA_SINGLE_CMP1_bm |
	                      TCA_SINGLE_CMP0_bm | TCA_SINGLE_CMP2_bm;

	TCA0.SINGLE.CMP0 = DEBOUNCE_TIME_ticks;
	TCA0.SINGLE.CMP1 = MAX_PULSE_ticks;
	TCA0.SINGLE.CMP2 = UART_RX_TIME_ticks;

	TCA0.SINGLE.PER = MIN_PERIOD_ticks;
}

// Starts the timer to check for pulse events
inline static void startTimer() {
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV8_gc | TCA_SINGLE_ENABLE_bm;
}

// Stops and rearm the timer
inline static void stopAndResetTimer() {
	TCA0.SINGLE.CTRLA = 0;

	TCA0.SINGLE.CNTH = 0;
	TCA0.SINGLE.CNTL = 0;
}

struct UARTIncoming uart = {.next = 0};

static volatile enum State state = IDLE;

static inline void initUART() {
	// Receive interrupt flag
	USART0.BAUD  = (uint16_t)(2000); // sets the baud rate to 20kBps
	// The datassheet speaks about integral and fractional part... It seems
	// inexistant, we just use integral part here.
	USART0.CTRLA = USART_RS485_OFF_gc;
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc |
	               USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;
	USART0.CTRLB = USART_RXEN_bm | USART_RXMODE_NORMAL_gc;
}

static inline void initPin7Montinoring() {
	PORTA.PIN7CTRL = PORT_ISC_BOTHEDGES_gc;
}

static inline void init() {
	// enable clock change
	CCP               = CCP_IOREG_gc;
	// Enable a 10MHz clock for a 3.3V power supply.
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm;
	CCP               = 0; // Protect the clock again.

	PORTA.DIRSET = PIN_DEBUG;
	PORTA.OUTCLR = PIN_DEBUG;

	initPin7Montinoring();
	initTimer();
	initUART();

	sei();
}

void processUART() {
	if ((USART0.STATUS & USART_RXCIE_bm) == 0) {
		// No data in the buffer
		return;
	}

	uint8_t status = USART0.RXDATAH;
	uint8_t data   = USART0.RXDATAL;

	if ((status & USART_FERR_bm) != 0) {
		return;
	}

	if (uart.next == 0 && data != 0x55) {
		return;
	}

	uart.Buffer[uart.next++] = data;

	if (uart.next < 3) {
		return;
	}
	uart.next = 0;

	// TODO: do something with incoming buffer
}

ISR(PORTA_PORT_vect) {

	bool upEdge = (PORTA.IN & PIN_IN) != 0x00;

	switch (state) {
	case IDLE:
		if (!upEdge) {
			startTimer();
			state = DEBOUNCE;
		}
		break;
	case DEBOUNCE:
		state = UART_RX;
		break;
	case PULSE:
		state = REST;
		break;
	case UART_RX:
	case REST:
		// we ignore in both case any change
		break;
	}
	// we clear the flag
	PORTA.INTFLAGS = PIN_IN;
}

ISR(TCA0_CMP0_vect) {
	if (state == DEBOUNCE) {
		state = PULSE;
	}

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCA0_CMP1_vect) {
	if (state == PULSE) {
		state = REST;
	}

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP1_bm;
}

ISR(TCA0_CMP2_vect) {
	if (state == UART_RX) {
		state = REST;
	}

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP2_bm;
}

ISR(TCA0_OVF_vect) {
	stopAndResetTimer();
	state = IDLE;

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main() {
	init();

	while (1) {
		processUART();
	}
}
