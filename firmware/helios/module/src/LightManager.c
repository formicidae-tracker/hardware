#include "LightManager.h"

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>



typedef void (*BrightnessSetter)(uint8_t);

typedef struct {
	bool active;
	bool IR_ready;
	uint8_t backupValues[NUM_CHANNELS];
	BrightnessSetter setters[NUM_CHANNELS];
} LightManager_t;

volatile LightManager_t LM;

#define UV_SET() PORTB |= _BV(1)
#define UV_CLEAR() PORTB &= ~(_BV(1))

#define VIS_SET() PORTB |= _BV(2);
#define VIS_CLEAR() PORTB &= ~(_BV(2))

#define SET_9BIT_PWM_BRIGHTNESS(Timer,Channel,Port,Pin,value) do {	  \
	if ( value == 0 ) { \
		OCR ## Timer ## Channel = 0; \
		TCCR ## Timer ## A &= ~(_BV(COM ## Timer ## Channel ## 1)); \
		PORT ## Port &= ~(_BV(Pin)); \
	} else { \
		OCR ## Timer ## Channel = (((uint16_t)value) << 1) + 1; \
		TCCR ## Timer ## A |= _BV(COM ## Timer ## Channel ## 1); \
	} \
}while(0)


#define IR_SET() PORTD |= _BV(5);
#define IR_CLEAR() PORTD &= ~(_BV(5));

#define TIMER3_START() do{	  \
		TCCR3B =  _BV(CS32); \
	}while(0)
#define TIMER3_STOP() do{	  \
		TCCR3B = 0x00; \
	}while(0)

#define FOR_ALL_CHANNELS(i) for (uint8_t i = 0; i < NUM_CHANNELS; ++i)


//forward declarations
void LMSetUVBrightness(uint8_t);
void LMSetVisibleBrightness(uint8_t);
void LMSetIRBrightness(uint8_t);

void InitLightManager() {
	cli();

	LM.active = false;
	LM.IR_ready = true;
	FOR_ALL_CHANNELS(c) {
		LM.backupValues[c] = 0;
	}

	//sets the right pin as output;
	DDRB |= _BV(1) | _BV(2); //Sets UV and Visible as output
	DDRD |= _BV(5); //sets IR as output


	LM.setters[UV] = &LMSetUVBrightness;
	LM.setters[VISIBLE] = &LMSetVisibleBrightness;
	LM.setters[IR] = &LMSetIRBrightness;
	// Sets TC1 as PWM source for Visible and UV
	// TPS92512 requires a PWM frequency between 100Hz and 1kHz
	// If we choose a fast PWM, we have F =1 / (N * (TOP+1)) N in 1,8,64,256,1024
	// with N == 64 and TOP=511 we got a frequency of 610 Hz
	// sets a fast pwm mode, disconnect all compare output, and 1/64
	TCCR1A = _BV(WGM11);
	TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS00);
	SET_9BIT_PWM_BRIGHTNESS(1, A, B, 1, 0);
	SET_9BIT_PWM_BRIGHTNESS(1, B, B, 2, 0);
	// starts the timer

	UV_CLEAR();
	IR_CLEAR();
	VIS_CLEAR();

	// Set Timer3 to measure IR pulses
	// Normal mode, timer prescaled 1/256, we want maximu duty of 0.25 and max pulse of 6.664ms
	// Duty ratio of 0.25 max
	OCR3A = 2*256*4; // A sets the period between pulse, we want no more than a duty ratio of 0.25
	OCR3B = 2*256; // B sets the pulse length 6.5664ms

	TIMSK3 = _BV(OCIE3B) | _BV(OCIE3A);
	TCCR3A = 0 ;
	TIMER3_STOP();


	//trigger interrupt on INT1 rising and on INT0 falling
	DDRD &= ~(_BV(2) | _BV(3) ) ; //makes sure INT1 and INT0 are input
	// Sets 1 as as rising and 0 as falling
	EICRA |= _BV(ISC11) | _BV(ISC10) | _BV(ISC01);
	EIMSK |= _BV(INT1) |_BV(INT0);

	sei(); // we need interrupt enabled
}



void LMSetUVBrightness(uint8_t value) {
	SET_9BIT_PWM_BRIGHTNESS(1, A, B, 1, value);
}

void LMSetVisibleBrightness(uint8_t value) {
	SET_9BIT_PWM_BRIGHTNESS(1, B, B, 2, value);
}


//Starts a pulse, i.e. marks the IR as unarmed, starts the pulse, and
//starts the counting.
#define LM_START_PULSE() do { \
		LM.IR_ready = false; \
		IR_SET(); \
		TIMER3_START(); \
	}while(0)


ISR(INT0_vect){
	//falling edge
	if (TCNT3 <=3) {
		return;
	}
	IR_CLEAR();
}

ISR(INT1_vect) {
	// rising edge
	if ( (!LM.IR_ready && TCNT3 > 3) || !LM.active ) {
		//ensure armed and active
		IR_CLEAR(); // to be sure
		return;
	}
	LM_START_PULSE();
}

ISR(TIMER3_COMPA_vect) {
	TIMER3_STOP();
	// enough time as ellapsed we re-arm the flash
	TCNT3 = 0;
	LM.IR_ready = true;
}

ISR(TIMER3_COMPB_vect){
	//no matter what, if we reach this we should stop to avoid to
	//exceed the maximal duty ratio
	IR_CLEAR();
}

void LMStartPulse() {
	uint8_t sreg = SREG;
	cli();
	if (!LM.IR_ready || !LM.active ) {
		IR_CLEAR();
		SREG = sreg;
		return;
	}
	LM_START_PULSE();
	SREG =sreg;
}

void LMSetIRBrightness(uint8_t value) {
	uint8_t sreg = SREG;
	cli();

	if (value == 0 ) {
		OCR3B = 2*256; // 6.528ms;
	} else {
		OCR3B = ((uint16_t)value) << 1;
	}
	SREG = sreg;
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
	LM.backupValues[UV] = (uint8_t)(OCR1A >> 1);
	LM.backupValues[VISIBLE] = (uint8_t)(OCR1B >> 1);
	if ( OCR1B == 2*256) {
		LM.backupValues[IR] = 0;
	} else {
		LM.backupValues[IR] = (uint8_t)(OCR3B >> 1);
	}
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
