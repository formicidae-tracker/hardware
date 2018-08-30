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

} LightManager_t;

volatile LightManager_t LM;
volatile uint16_t systime;


#define UV_SET() PORTB |= _BV(1)
#define UV_CLEAR() PORTB &= ~(_BV(1))

#define VIS_SET() PORTB |= _BV(2);
#define VIS_CLEAR() PORTB &= ~(_BV(2))

#define IR_SET() PORTD |= _BV(5);
#define IR_CLEAR() PORTD &= ~(_BV(5));

#define TIMER3_START() TCCR3B = _BV(WGM12) | _BV(CS11) | _BV(CS10) //sets 1/64 prescaling
#define TIMER3_STOP() TCCR3B = _BV(WGM12) //sets 1/64 prescaling

#define FOR_ALL_CHANNELS(i) for (uint8_t i = 0; i < NUM_CHANNELS; ++i)

void LMSetVisibleBrightness(uint8_t value) {
	OCR1B = value;
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

//Starts a pulse, i.e. marks the IR as unarmed, starts the pulse, and
//starts the counting.
#define LM_START_PULSE() do { \
		LM.IR_ready = false; \
		TCNT1 = 0; \
		IR_SET(); \
		TIMER1_START(); \
	}while(0)


ISR(PCINT0_vect){
	// if we are not ready, or if we are not active, or its a falling
	// edge, we clear the output.
	if ( !LM.IR_ready || !LM.active || (PIND & _BV(3)) == 0) {
		IR_CLEAR();
		return;
	}
	//rising edge, active and armed, we start a flash
	LM_START_PULSE();
}

void LMStartPulse() {
	uint8_t sreg = SREG;
	cli();
	if (!LM.IR_ready || !LM.active ) {
		IR_CLEAR();
		return;
	}
	LM_START_PULSE();
	SREG =sreg;
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
	systime = 0;
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
	DDRB |= _BV(2); //Sets Visible as output
	PORTB &= ~(_BV(2));
	DDRA |= _BV(2) | _BV(0); //sets UV and IR as output
	PORTA &= ~(_BV(2) | _BV(0) );
	// sets a fast pwm mode, disconnect all compare output, and 1/64
	TCCR0A = _BV(WGM01) | _BV(WGM00);
	TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);
	OCR0A = 0;
	OCR0B = 0;
	// starts the timer
	TCCR0B = _BV(CS01) | _BV(CS00);

	// CTC mode, timer prescaled 1/64, we want maximu duty of 0.25 and max pulse of 6.528ms
	// Duty ratio of 0.25 max
	OCR1A = 4*256*4; // A sets the period between pulse, we want no more than a duty ratio of 0.25
	OCR1B = 4*256; // B sets the pulse length 6.528ms

	TIMSK1 = _BV(OCIE1B) | _BV(OCIE1A);
	TCCR1A = 0 ;
	TIMER1_STOP();

	//enable strobe pulse detection
	//PCMSK0 |= _BV(7);

	sei(); // we need interrupt enabled
}

ISR(TIM0_OVF_vect) {
	if (LM.UV != 0) {
		UV_SET();
	}
	++systime;
}

ISR(TIM0_COMPB_vect) {
	UV_CLEAR();
}

void LMActivateOutput() {
	if (LM.active == true ) {
		return;
	}
	uint8_t sreg = SREG;
	cli();
	LM.active = true;
	FOR_ALL_CHANNELS(c) {
		LM.setters[c](LM.backupValues[c]);
	}
	SREG = sreg;
}

void LMDeactivateOutput() {
	if (LM.active == false ) {
		return;
	}

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
	uint16_t res = systime;
	SREG = sreg;
	return res;
}
