/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"


#include "gpio.h"


void pinMode(uint8_t pin, uint8_t mode)
{
	uint32_t bit = digitalPinToBitMask(pin);	//changed to be 32bits instead of 8bits 
	//uint8_t port = digitalPinToPort(pin);	//no ports
	volatile uint32_t *reg;	//changed to be 32bits instead of 8bits 

	//if (port == NOT_A_PIN) return;
	if (pin >= NUM_DIGITAL_PINS) return;	//check for number of pin

	// JWS: can I let the optimizer do this?
	reg = REGP(GPIO_REG_PADDIR);	//not the best practice, ### check later###

	if (mode == INPUT) { 
		//uint8_t oldSREG = SREG;	//interrupt ##check later###
                //cli();	//interrupt ##check later###
		*reg &= ~bit;
		//SREG = oldSREG;	//interrupt ##check later###
	}
//////////////////////// Supression///////////////////////
//We don't have pullup and pulldown for FPGA but have it for imperio, ###check later###
/*
	else if (mode == INPUT_PULLUP) {
		uint8_t oldSREG = SREG;
                cli();
		*reg &= ~bit;
		*out |= bit;
		SREG = oldSREG;
	}
*/
/////////////////////////////End////////////////////////////	
	else {
		//uint8_t oldSREG = SREG;	//interrupt ##check later###
                //cli();	//interrupt ##check later###
		*reg |= bit;
		//SREG = oldSREG;	//interrupt ##check later###
	}
}

//////////////////////// Supression///////////////////////
//Timers Stuff, ###check later###
/*
// Forcing this inline keeps the callers from having to push their own stuff
// on the stack. It is a good performance win and only takes 1 more byte per
// user than calling. (It will take more bytes on the 168.)
//
// But shouldn't this be moved into pinMode? Seems silly to check and do on
// each digitalread or write.
//
// Mark Sproul:
// - Removed inline. Save 170 bytes on atmega1280
// - changed to a switch statment; added 32 bytes but much easier to read and maintain.
// - Added more #ifdefs, now compiles for atmega645
//
//static inline void turnOffPWM(uint8_t timer) __attribute__ ((always_inline));
//static inline void turnOffPWM(uint8_t timer)


static void turnOffPWM(uint8_t timer)
{
	switch (timer)
	{
		#if defined(TCCR1A) && defined(COM1A1)
		case TIMER1A:   cbi(TCCR1A, COM1A1);    break;
		#endif
		#if defined(TCCR1A) && defined(COM1B1)
		case TIMER1B:   cbi(TCCR1A, COM1B1);    break;
		#endif
		#if defined(TCCR1A) && defined(COM1C1)
		case TIMER1C:   cbi(TCCR1A, COM1C1);    break;
		#endif
		
		#if defined(TCCR2) && defined(COM21)
		case  TIMER2:   cbi(TCCR2, COM21);      break;
		#endif
		
		#if defined(TCCR0A) && defined(COM0A1)
		case  TIMER0A:  cbi(TCCR0A, COM0A1);    break;
		#endif
		
		#if defined(TIMER0B) && defined(COM0B1)
		case  TIMER0B:  cbi(TCCR0A, COM0B1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2A1)
		case  TIMER2A:  cbi(TCCR2A, COM2A1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2B1)
		case  TIMER2B:  cbi(TCCR2A, COM2B1);    break;
		#endif
		
		#if defined(TCCR3A) && defined(COM3A1)
		case  TIMER3A:  cbi(TCCR3A, COM3A1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3B1)
		case  TIMER3B:  cbi(TCCR3A, COM3B1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3C1)
		case  TIMER3C:  cbi(TCCR3A, COM3C1);    break;
		#endif

		#if defined(TCCR4A) && defined(COM4A1)
		case  TIMER4A:  cbi(TCCR4A, COM4A1);    break;
		#endif					
		#if defined(TCCR4A) && defined(COM4B1)
		case  TIMER4B:  cbi(TCCR4A, COM4B1);    break;
		#endif
		#if defined(TCCR4A) && defined(COM4C1)
		case  TIMER4C:  cbi(TCCR4A, COM4C1);    break;
		#endif			
		#if defined(TCCR4C) && defined(COM4D1)
		case TIMER4D:	cbi(TCCR4C, COM4D1);	break;
		#endif			
			
		#if defined(TCCR5A)
		case  TIMER5A:  cbi(TCCR5A, COM5A1);    break;
		case  TIMER5B:  cbi(TCCR5A, COM5B1);    break;
		case  TIMER5C:  cbi(TCCR5A, COM5C1);    break;
		#endif
	}
}

*/
/////////////////////////////End////////////////////////////

void digitalWrite(uint8_t pin, uint8_t val)
{
	//uint8_t timer = digitalPinToTimer(pin);	//timer ##check later###
	uint32_t bit = digitalPinToBitMask(pin);	//changed to be 32bits instead of 8bits
	//uint8_t port = digitalPinToPort(pin);	// no ports
	volatile uint32_t *out;

	//if (port == NOT_A_PIN) return;
	if (pin >= NUM_DIGITAL_PINS) return;	//check for number of pin

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	//if (timer != NOT_ON_TIMER) turnOffPWM(timer);	//timer ##check later###

	out = REGP(GPIO_REG_PADOUT);	//not the best practice, ### check later###	

	//uint8_t oldSREG = SREG;	//interrupt ##check later###
	//cli();	//interrupt ##check later###

	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}

	//SREG = oldSREG;	//interrupt ##check later###
}

int digitalRead(uint8_t pin)
{
	//uint8_t timer = digitalPinToTimer(pin);	//timer ##check later###
	uint32_t bit = digitalPinToBitMask(pin);	//changed to be 32bits instead of 8bits
	//uint8_t port = digitalPinToPort(pin);	// no ports

	//if (port == NOT_A_PIN) return LOW;
	if (pin >= NUM_DIGITAL_PINS) return LOW;	//check for number of pin

	// If the pin that support PWM output, we need to turn it off
	// before getting a digital reading.
	//if (timer != NOT_ON_TIMER) turnOffPWM(timer);	//timer ##check later###

	if (*REGP(GPIO_REG_PADIN) & bit) return HIGH;	//not the best practice, ###Check late###
	return LOW;
}
