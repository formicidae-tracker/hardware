#include <stdbool.h>
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define PIN_DEBUG _BV(1)
#define PIN_IN    _BV(7)
#define PIN_ADDR  _BV(3)
#define PIN_OUT   _BV(2)

struct UARTIncoming {
	char    Buffer[3];
	uint8_t next, offset;
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

// Inits the PWM waveform generation on A1 and A2
inline static void initPWM() {
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc |
	                    TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_CMP1EN_bm;

	TCA0.SINGLE.CMP2 = 0;
	TCA0.SINGLE.CMP1 = 0;

	// we limit the max PWM at 50% to protect the stip. CC wasn't a bright idea.

	TCA0.SINGLE.PER   = 510;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
}

//  Sets the value for the PWM waveform.
void setPWM(uint8_t value) {
	TCA0.SINGLE.CMP2BUF = value;
	TCA0.SINGLE.CMP1BUF = value;
}

struct UARTIncoming uart = {.next = 0};

// Initialize a 20kbps RX UART.
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

// Initializes a 1ms period RTC.
static inline void initRTC() {
	RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;
	while ((RTC.STATUS & RTC_CTRLABUSY_bm) != 0x00) {
	}
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc | RTC_RTCEN_bm;
}

// Get the clock time. Used for debug waveform generation.
static inline uint16_t getRTC() {
	return RTC.CNT;
}

// Global initialization routine.
static inline void init() {
	// enable clock change
	CCP               = CCP_IOREG_gc;
	// Enable a 10MHz clock for a 3.3V power supply.
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm;
	CCP               = 0; // Protect the clock again.

	// set pullup ion addr pin.
	PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTA.DIRSET   = PIN_DEBUG | PIN_OUT;
	PORTA.OUTCLR   = PIN_DEBUG | PIN_OUT;

	initPWM();
	initUART();
	initRTC();

	if ((PORTA.IN & PIN_ADDR) == 0x00) {
		uart.offset = 2;
	} else {
		uart.offset = 1;
	}
}

// Process incoming UART packet.
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

	// setPWM from value.
	setPWM(uart.Buffer[uart.offset]);
}

// Main loop that generate a full pulse ( for now).
#define PERIOD_ms 10000

int main() {
	init();
	uint16_t last = 0;
	while (1) {
		processUART();
		/* uint16_t now = getRTC(); */

		/* if ((now - last) < 40) { */
		/* 	continue; */
		/* } */
		/* last += 40; */

		/* int16_t phase = now % PERIOD_ms; */
		/* if (phase >= PERIOD_ms / 2) { */
		/* 	phase = PERIOD_ms - phase; */
		/* } */

		/* setPWM(((uint32_t)(510) * (uint32_t)phase) / (uint32_t)PERIOD_ms); */
	}
}
