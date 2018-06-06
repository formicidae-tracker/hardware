#include "ChargeMonitor.h"

#include <avr/io.h>
#include <avr/interrupt.h>

typedef struct {
	bool lastStatus;
} ChargeMonitor_t;

ChargeMonitor_t CM;

void InitChargeMonitor() {
	CM.lastStatus = false;

	ADMUX |= _BV(MUX1); //PA1 is considered as a singled ended input
	DIDR0 |= _BV(ADC1D); // disable PA1 as an digital input

	//enables ADC with a prescaler of 128 -> F = 78125Hz, and starts the first conversion
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

}


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
	if (CM.lastStatus == false && value >= 0xefff) {
		CM.lastStatus = true;
		//TODO Enable 36V
	} else if ( CM.lastStatus == true && value <= 0xdfff ) {
		CM.lastStatus = false;
		//TODO disable 36V
	}

	return CM.lastStatus;
}
