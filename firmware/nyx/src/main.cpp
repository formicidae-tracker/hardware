#include "system_clock.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

class IRController {
public:
	static constexpr auto MIN_PERIOD    = 1024; // we can go up to 32Hz
	static constexpr auto MAX_PULSE     = USToTicks(4500);
	static constexpr auto UART_RX_TIME  = USToTicks(2000);
	// clock is low to start, so only one tick is sufficient. Ideally should be
	// 5
	static constexpr auto DEBOUNCE_TIME = 1;

	static constexpr uint8_t pinOut = 2;
	static constexpr uint8_t pinIn  = 7;

	enum class State {
		IDLE,
		PULSE,
		REST,
		PULSE_DEBOUNCE,
		UART_RX,
	};

	IRController()
	    : d_state{State::REST} {
		outLow();
		PORTA.DIRSET = _BV(pinOut);
		PORTA.DIRCLR = _BV(pinIn);
		system_clock_start();
	}

	inline void Work(absolute_time_t now) {

		switch (d_state) {
		case State::IDLE:
			// we check for an high to low transition.
			if (inLow() == true) {
				d_state = State::PULSE_DEBOUNCE;
				system_clock_start();
			}
			break;
		case State::REST:
			// we wait for sufficient time to pass before rearming.
			if (now >= MIN_PERIOD) {
				d_state = State::IDLE;
				system_clock_stop();
			}
			break;
		case State::PULSE: {
			// we wait for a low to high, or max pulse is reached.
			bool inPulseDone     = !inLow();
			bool maxPulseReached = now >= MAX_PULSE;
			if (inPulseDone || maxPulseReached) {
				outLow();
				d_state = State::REST;
			}
			break;
		}
		case State::PULSE_DEBOUNCE:
			// we are testing if the pulse is an IR trigger or an UART packet

			if (inLow() == false) {
				// start condition and the first two received bits should be
				// zero:
				// its a pulse, if a bit is flipped, we reject it as noise or
				// most likely an UART TX.
				d_state = State::UART_RX;
			}
			if (now >= DEBOUNCE_TIME) {
				// it is a pulse
				on(now);
				// the actual pulse start is now.
			}
			break;
		case State::UART_RX:
			// We wait for 3 bytes to be fully received before scanning for
			// input.
			if (now < UART_RX_TIME || inLow() == false) {
				return;
			}
			if (now > MIN_PERIOD || now == 0) {
				d_state = State::IDLE;
			} else {
				d_state = State::REST;
			}
			break;
		}
	}

private:
	inline void on(absolute_time_t now) {
		d_state = State::PULSE;
		outHigh();
	}

	inline void off() {
		outLow();
		d_state = State::REST;
	}

	inline void outHigh() {
		PORTA.OUTSET = _BV(pinOut) | _BV(1);
	}

	inline void outLow() {
		PORTA.OUTCLR = _BV(pinOut) | _BV(1);
	}

	inline bool inLow() const {
		return (PORTA.IN & _BV(pinIn)) == 0;
	}

	State           d_state;
};

static IRController ir;

inline void init() {
	// enable clock change
	CCP               = CCP_IOREG_gc;
	// Enable a 5MHz clock for a 3.3V power supply.
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm;
	CCP               = 0; // Protect the clock again.

	init_system_clock();

	PORTA.DIRSET = _BV(6) | _BV(1); // set A1 and A6 as output

	// Receive interrupt flag
	USART0.BAUD  = uint16_t(2000); // sets the baud rate to 20kBps
	// The datassheet speaks about integral and fractional part... It seems
	// inexistant, we just use integral part here.
	USART0.CTRLA = USART_RS485_OFF_gc;
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc |
	               USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;
	USART0.CTRLB = USART_RXEN_bm | USART_RXMODE_NORMAL_gc;
	cli();
}

constexpr static uint8_t RX_BUFFER_SIZE = 3;
static char    rx_buffer[RX_BUFFER_SIZE];
static uint8_t next = 0;

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

	if (next == 0 && data != 0x55) {
		return;
	}

	rx_buffer[next++] = data;

	if (next < 3) {
		return;
	}
	next = 0;

	// we got a frame! we can ignore it!!!
}

int main() {
	init();
	cli();
	for (;;) {
		auto now = get_absolute_time();
		ir.Work(now);

		processUART();
	}
	return 0;
}
