#include "system_clock.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

class IRController {
public:
	static constexpr auto MIN_PERIOD = MSToTicks(1.0f / 30.5f);
	static constexpr auto MAX_PULSE  = MSToTicks(3.0f);
	enum class State {
		IDLE,
		PULSE,
		REST,
	};

	IRController()
	    : d_state{State::REST}
	    , d_pulseStart{get_absolute_time()} {
		outLow();
		PORTA.DIRSET = _BV(2);
	}

	inline void Work(absolute_time_t now) {

		switch (d_state) {
		case State::REST:
			if ((now - d_pulseStart) > MIN_PERIOD) {
				d_state = State::IDLE;
			}
			break;
		case State::PULSE: {
			bool inPulseDone     = (PORTA.IN & _BV(7)) != 0;
			bool maxPulseReached = (now - d_pulseStart) > MAX_PULSE;
			if (inPulseDone || maxPulseReached) {
				outLow();
				d_state = State::REST;
			}
			break;
		}
		case State::IDLE:
			break;
		}
	}

	void On() {
		if (d_state != State::IDLE) {
			return;
		}
		d_state      = State::PULSE;
		d_pulseStart = get_absolute_time();
		outHigh();
	}

	void Off() {
		if (d_state != State::PULSE) {
			return;
		}
		outLow();
		d_state = State::REST;
	}

private:
	inline void outHigh() {
		PORTA.OUTSET = _BV(2);
	}

	inline void outLow() {
		PORTA.OUTCLR = _BV(2);
	}

	State           d_state;
	absolute_time_t d_pulseStart;
};

static IRController ir;

inline void init() {
	// enable clock change
	CCP               = CCP_IOREG_gc;
	// Enable a %MHz clock for a 3.3V power supply.
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_4X_gc | CLKCTRL_PEN_bm;
	CCP               = 0; // Protect the clock again.

	init_system_clock();

	PORTA.DIRSET = _BV(7) | _BV(1); // set A1 and A7 as output

	// Receive interrupt flag

	USART0.CTRLA = USART_RXCIE_bm;
	USART0.CTRLB = USART_RXEN_bm | USART_RXMODE_NORMAL_gc;
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc |
	               USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;
	USART0.BAUD = uint16_t(1000) << 6; // sets the baud rate to 20kBps
}

constexpr static uint8_t RX_BUFFER_SIZE = 4;
static char    rx_buffer[RX_BUFFER_SIZE];
static uint8_t next = 0;

ISR(USART0_RXC_vect) {
	uint8_t ctrl = USART0.RXDATAH;
	uint8_t data = USART0.RXDATAL;

	if ((ctrl & USART_FERR_bm) != 0) {
		// we got a frame error. Happens on a pulse.
		if (data == 0x00) {
			// this is indeed a pulse. we start the actual pulse
			ir.On();
		} else {
			// erroneous frame, ignore it all
			next = 0;
		}
		return;
	}
	if (next == 0 & data != 0xaa) {
		// wrong header.
		return;
	}
	if (next < RX_BUFFER_SIZE) {
		rx_buffer[next++] = data;
	}
}

int main() {
	init();

	for (;;) {
		auto now = get_absolute_time();
		ir.Work(now);

		if (next == RX_BUFFER_SIZE) {
			// TODO: do something
			next = 0;
		}
	}
	return 0;
}
