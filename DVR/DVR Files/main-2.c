 /************************************************************************/
 /* main.c - EGB240 - Group 6 Milestone Demonstration                    */
 /*                                                                      */
 /************************************************************************/

 /************************************************************************/
 /* INCLUDED LIBRARIES/HEADER FILES                                      */
 /************************************************************************/
 #include <avr/io.h>
 #include <avr/interrupt.h>

 #include <stdio.h>

 #include "serial.h"

 #define TOP 255



 void PwM_start(scaler = 0b00000011 , top = TOP , DC = 128)
 {

	 cli();

	 // Configure system clock for 16 MHz
	 CLKPR = 0x80;	// Prescaler change enable
	 CLKPR = 0x00;	// Prescaler /1, 16 MHz

	 // Initialize Ports for use
	 DDRF &= 0b10001111;    // Pushbuttons 1 to 3 - PORTF 6-4 as inputs
	 DDRD |= 0b11110000;		// Set PORTD 7-4 as outputs (LEDs)	 
	 DDRB |= 0b01000000;	   // JOUT - PORTB 6 as an output
	 

	 // Initialize Timer4 for 8-bit PWM
	 // Using OC4B, connected to B6 (JOUT)
	 TCCR4B = scaler;  //prescaler 4, 15.625kHz
	 TCCR4A = 0x21;   // bits 5 & 4 set clear on CMP, bit 0 enables PWM // TCCR4A FOR OCR4D
	 OCR4C = top;    // set top to 0xFF (255)
	 OCR4B = DC;   // initialize to DC duty cycle. Default DC = 50%. 
	 TIMSK4 = 0x04;	// Timer/Counter4 Overflow Interrupt Enable
	 TCNT4 = 0x00;  // reset timer
	 


	 serial_init();	// Initialise USB serial interface (debug)

	 sei();
 }

	ISR(TIMER4_OVF_vect) {
	uint8_t fidgit	 = buffer_dequeue();		//dequeue here
	OCR4B = fidgit;
	}//ISR