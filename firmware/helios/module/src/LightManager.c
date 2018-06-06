#include "LightManager.h"

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>



typedef void (*BrightnessSetter)(uint8_t);

typedef struct {
	bool active;
	uint8_t UV,Visible;
	BrightnessSetter setters[NUM_CHANNELS];

} LightManager_t;

LightManager_t LM;


#define UV_SET() PORTA |= _BV(0)


#define UV_CLEAR() PORTA &= ~(_BV(0))


#define VIS_CLEAR() PORTB &= ~(_BV(2))

void LMSetVisibleBrightness(uint8_t value) {
	if(value == 0 ) {
		TCCR0A = _BV(WGM01) | _BV(WGM00);
		VIS_CLEAR();
		return;
	}
	//enables OC0A
	TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
	OCR0A = value;

}

void LMSetUVBrightness(uint8_t value) {
	if ( value == 0xff ) {
		TIMSK0 = _BV(TOIE0);
		UV_SET();
	}
	TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);
	int sreg = SREG;
	cli();
	LM.UV = value;
	OCR0B = value;
	SREG = sreg;
}

void LMSetIRBrightness(uint8_t value) {
}




void InitLightManager() {
	LM.active = false;
	LM.UV = 0;
	LM.Visible = 0;

	LM.setters[UV] = &LMSetUVBrightness;
	LM.setters[VISIBLE] = &LMSetVisibleBrightness;
	LM.setters[IR] = &LMSetIRBrightness;
	// Sets TC0 as PWM source for Visible and UV
	// TPS92512 requires a PWM frequency between 100Hz and 1kHz
	// If we choose a fast PWM, we have F =1 / (N * 256) N in 1,8,64,256,1024
	// with N == 64 we got a frequency of 610 Hz

	//sets the right pin as output;
	PORTB &= ~(_BV(2));
	DDRB |= _BV(2); //Sets Visible as output
	PORTA &= ~(_BV(2) | _BV(0) );
	DDRA |= _BV(2) | _BV(0); //sets UV and IR as output

	// sets a fast pwm mode, disconnect all compare output, and 1/64
	TCCR0A = _BV(WGM01) | _BV(WGM00);
	TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);

	// starts the timer
	TCCR0B = _BV(CS01) | _BV(CS00);
}

ISR(TIM0_OVF_vect) {
	if (LM.UV != 0) {
		UV_SET();
	}
}


ISR(TIM0_COMPB_vect) {
	UV_CLEAR();
}

void LMActivateOutput() {
	LM.active = true;
}

void LMDeactivateOutput() {
	LM.active = false;
}



void LMSetBrightness(channel_e channel, uint8_t brightness) {
	if (channel >= NUM_CHANNELS) {
		return;
	}
	LM.setters[channel](brightness);
}
