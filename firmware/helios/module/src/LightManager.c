#include "LightManager.h"

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>



typedef void (*BrightnessSetter)(uint8_t);

typedef struct {
	bool active;
	uint8_t UV;
	bool IR_ready;
	uint8_t backupValues[NUM_CHANNELS];
	BrightnessSetter setters[NUM_CHANNELS];
	uint16_t systime;

} LightManager_t;

LightManager_t LM;


#define UV_SET() PORTA |= _BV(0)
#define UV_CLEAR() PORTA &= ~(_BV(0))

#define VIS_SET() PORTB |= _BV(2);
#define VIS_CLEAR() PORTB &= ~(_BV(2))

#define IR_SET() PORTA |= _BV(2);
#define IR_CLEAR() PORTA &= ~(_BV(2));

#define TIMER1_START() TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10) //sets 1/64 prescaling
#define TIMER1_STOP() TCCR1B = _BV(WGM12) //sets 1/64 prescaling

#define FOR_ALL_CHANNELS(i) for (uint8_t i = 0; i < NUM_CHANNELS; ++i)

void LMSetVisibleBrightness(uint8_t value) {
	OCR0A = value;
	if(value == 0 ) {
		TCCR0A = _BV(WGM01) | _BV(WGM00);
		VIS_CLEAR();
		return;
	}
	//enables OC0A
	TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
}

void LMSetUVBrightness(uint8_t value) {
	if ( value == 0xff ) {
		TIMSK0 = _BV(TOIE0);
		UV_SET();
	}
	TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);
	int sreg = SREG;
	cli();
	OCR0B = value;
	SREG = sreg;
}

ISR(PCINT0_vect){
	// if we are not ready, or if we are not active, or its a falling
	// edge, we clear the output.
	if ( !LM.IR_ready || !LM.active || (PINA & _BV(7)) == 0) {
		IR_CLEAR();
		return;
	}
	//rising edge, active and armed, we start a flassh

	//we mark next trigger as unready
	LM.IR_ready = false;

	TCNT1 = 0;
	IR_SET();
	//starts the TIMER counter
	TIMER1_START();
}

ISR(TIM1_COMPA_vect) {
	TIMER1_STOP();
	// enough time as ellapsed we re-arm the flash
	LM.IR_ready = true;

}

ISR(TIM1_COMPB_vect){
	//no matter what, if we reach this we should stop !
	IR_CLEAR();
}

void LMSetIRBrightness(uint8_t value) {
	uint8_t sreg = SREG;
	cli();

	if (value == 0 ) {
		OCR1B = 4*256; // 6.528ms;
	} else {
		OCR1B = ((uint16_t)value) << 2;
	}
	SREG = sreg;
}

void InitLightManager() {
	cli();

	LM.active = false;
	LM.UV = 0;
	LM.IR_ready = true;
	LM.systime = 0;
	FOR_ALL_CHANNELS(c) {
		LM.backupValues[c] = 0;
	}

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

	// CTC mode, timer prescaled 1/64, we want maximu duty of 0.2 and max pulse of 6.528ms
	// Duty ratio of 0.2 max
	OCR1A = 4*256*4; // A sets the period between pulse, we want no more than a duty ratio of 0.2
	OCR1B = 4*256; // B sets the pulse length 6.528ms

	TIMSK1 = _BV(OCIE1B) | _BV(OCIE1A);
	TCCR1A = 0 ;
	TIMER1_STOP();


	//enable strobe pulse detection
	PCMSK0 |= _BV(7);

	sei(); // we need interrupt enabled
}

ISR(TIM0_OVF_vect) {
	if (LM.UV != 0) {
		UV_SET();
	}
	++LM.systime;
}

ISR(TIM0_COMPB_vect) {
	UV_CLEAR();
}

void LMActivateOutput() {
	uint8_t sreg = SREG;
	cli();
	LM.active = true;
	FOR_ALL_CHANNELS(c) {
		LM.setters[c](LM.backupValues[c]);
	}
	SREG = sreg;
}

void LMDeactivateOutput() {
	uint8_t sreg = SREG;
	cli();
	LM.backupValues[VISIBLE] = OCR0A;
	if ( OCR1B == 4*256) {
		LM.backupValues[IR] = 0;
	} else {
		LM.backupValues[IR] = (uint8_t)(OCR1B >> 2);
	}
	LM.backupValues[UV] = LM.UV;
	LM.active = false;
	FOR_ALL_CHANNELS(c) {
		LM.setters[c](0);
	}
	IR_CLEAR();
	VIS_CLEAR();
	UV_CLEAR();
	SREG = sreg;
}



void LMSetBrightness(channel_e channel, uint8_t brightness) {
	if (channel >= NUM_CHANNELS ) {
		return;
	}

	if (LM.active == false) {
		LM.backupValues[channel] = brightness;
		return;
	}

	LM.setters[channel](brightness);
}


uint16_t LMSystime() {
	uint8_t sreg = SREG;
	cli();
	uint16_t res = LM.systime;
	SREG = sreg;
	return res;
}