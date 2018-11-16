/*Copyright [2017] [Siddhant Mahapatra]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/Robosid/Electronics/blob/master/License.pdf
    https://github.com/Robosid/Electronics/blob/master/License.rtf

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/



/**
 * adc.c - EGB240DVR Library, ADC module
 *
 * Configures the ADC to sample on CH0 and store conversion
 * results into a circular buffer. Conversions are triggered
 * from the Timer0 CMPA signal.
 *
 * Requires:
 *   timer	- Configures Timer0 to trigger ADC conversions. 
 *   buffer - Circular buffer (queue) used to store audio samples.
 *
 * Version: v1.0
 *    Date: 05/29/2017
 *  Modified by: Sid 
 *  E-mail: robo_sid@yahoo.co.uk
 */ 

/************************************************************************/
/* INCLUDED LIBRARIES/HEADER FILES                                      */
/************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "buffer.h"

/************************************************************************/
/* PUBLIC/USER FUNCTIONS                                                */
/************************************************************************/
void adc_init() {
	ADMUX = 0x60;	// Left adjust result, AREF = AVCC
	ADCSRB = 0x03;	// Select Timer0 CMPA as trigger	
}

void adc_start() {
	ADCSRA = 0xAE;	// /64 prescaler (250 kHz clock), enable interrupts, ADC enable
}

void adc_stop() {
	ADCSRA = 0x00;
}

/************************************************************************/
/* INTERRUPT SERVICE ROUTINES                                           */
/************************************************************************/

/**
 * ISR: ADC conversion complete
 * 
 * Interrupt service routine which executes on completion of ADC conversion.
 */
ISR(ADC_vect) {
	uint8_t result = ADCH;	//Read result
	buffer_queue(result);	//Store result into buffer
}