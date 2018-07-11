#include "ChargeMonitor.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Debug.h"

typedef struct {
	bool lastStatus;
} ChargeMonitor_t;

ChargeMonitor_t CM;

void InitChargeMonitor() {
	CM.lastStatus = false;

	ADMUX |= _BV(MUX0); //PA1 is considered as a singled ended input, 5V reference
	DIDR0 |= _BV(ADC1D); // disable PA1 as an digital input

	//enables ADC with a prescaler of 128 -> F = 78125Hz, and starts the first conversion
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

	//EN36V as output 0 initial state
	DDRA |= _BV(3);
}

// startup threshold should be determined from strating current
// i_start = (Uin - Uo) / R we have three resistor 4.7 4.7 and 15 ohm,
// so we can assume than R could be 2 therefore if we do not want to
// exceed the current limit of 0.840 mA, we chould at leadt have 34.3V
// setting a threshold at 34.5V so 3.88V after 12.7/100 voltage
// divider, with a voltage reference of 5V, it should be 796, with a
// 2V hysteresis, low threshold should be 749

#define ON_THRESHOLD 796
#define OFF_THRESHOLD 749

bool CMCheckCharge() {
	//TODO : should enable EN36V, other wise all will go through a
	//large limiting resistor !!!


	// if adc conversion is currently runnning
	if ( (ADCSRA & _BV(ADSC)) != 0 ) {
		return CM.lastStatus;
	}

	uint8_t sreg = SREG;
	cli();
	uint16_t value = ADC;
	SREG = sreg;
	// starts ADC
	ADCSRA |= _BV(ADSC);

	//hysteresis on the input value
	if (CM.lastStatus == false && value >= ON_THRESHOLD ) {
		CM.lastStatus = true;
		PORTA |= _BV(3);
	} else if ( CM.lastStatus == true && value <=  OFF_THRESHOLD ) {
		CM.lastStatus = false;
		PORTA &= ~(_BV(3)); ;
	}
	Log(value>>2);

	return CM.lastStatus;
}
