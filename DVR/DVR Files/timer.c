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
 * timer.c - EGB240DVR Library, Timer module
 *
 * Configures Timer0 to generate regular interrupts for sampling 
 * and other timing purposes.
 *
 * The timer module sequences and triggers sampling of the ADC,
 * and is required for operation of the FAT file system module.
 * The timer may also be used to trigger other regular events.
 *
 * Requires:
 *   lib/fatfs - FatFs FAT file system library published by ChaN
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
 
#include "lib/fatfs/diskio.h"
 
#include "timer.h"

/************************************************************************/
/* GLOBAL VARIABLES                                                     */
/************************************************************************/
volatile uint8_t timer_fatfs = TIMER_INTERVAL_FATFS;	// Counter variable for servicing FatFs
volatile uint16_t timer_led = TIMER_INTERVAL_LED;		// Counter for debug LED flashing

/************************************************************************/
/* PUBLIC/USER FUNCTIONS                                                */
/************************************************************************/

/**
 * Function: timer_init
 * 
 * Initialises and starts Timer0 with a 64 us period (15.625 kHz).
 * Assumes a 16 MHz system clock. Interrupts at counter top.
 */
void timer_init() {
	OCR0A = 128;	// 15.625 kHz (64 us period)
	TCCR0A = 0x02;	// CTC mode
	TIMSK0 = 0x02;  // Interrupt on CMPA (top)
	
	TCCR0B = 0x02;  // Start timer, /8 prescaler

	DDRD |= (1<<PIND7);		// Set PORTD7 (LED4) as output
}

/************************************************************************/
/* INTERRUPT SERVICE ROUTINES                                           */
/************************************************************************/

/**
 * ISR: Timer0 CompareA Interrupt
 * 
 * Interrupt service routine for Timer0 CompareA vector.
 * Corresponds to top of timer for CTC mode.
 *
 * Used to generate regular, timed events.
 */
ISR(TIMER0_COMPA_vect) {
	
	// Timer to service FatFs module (~10 ms interval)
	if (!(--timer_fatfs)) {
		timer_fatfs = TIMER_INTERVAL_FATFS;
		disk_timerproc();
	}
	
	// Timer to flash debug LED (1 Hz, 50% duty cycle flash)
	if (!(--timer_led)) {
		timer_led = TIMER_INTERVAL_LED;
		//PORTD ^= (1<<PIND7);
	}
	
}