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
 * serial.c - EGB240DVR Library, Serial interface
 *
 * Provides a serial interface via the USB port.
 *
 * Allows the use of stdio library functions (printf etc.)
 * Can be used for debug, control and user interface purposes.
 *
 * Requires:
 *   lib/usb_serial - USB serial library published by PJRC.com
 *
  * Version: v1.0
 *    Date: 05/29/2017
 *  Modified by: Sid 
 *  E-mail: robo_sid@yahoo.co.uk
 */ 

/************************************************************************/
/* INCLUDED LIBRARIES/HEADER FILES                                      */
/************************************************************************/
#include <stdio.h>

#include "lib/usb_serial/usb_serial.h"

/************************************************************************/
/* PROTOTYPE FUNCTIONS                                                  */
/************************************************************************/
static uint8_t serial_putchar(char c, FILE *stream);
static uint8_t serial_getchar(FILE *stream);
static FILE stdinout = FDEV_SETUP_STREAM(serial_putchar, serial_getchar, _FDEV_SETUP_RW);

/************************************************************************/
/* PRIVATE/UTILLITY FUNCTIONS                                           */
/************************************************************************/
static uint8_t serial_putchar(char c, FILE *stream) {
	//outputs a character via the USB serial interface
	return usb_serial_putchar(c);
}

static uint8_t serial_getchar(FILE *stream) {
	//wait for character (blocking)
	while(!usb_serial_available());
	//read a character from the USB serial interface 
	return usb_serial_getchar();
}

/************************************************************************/
/* PUBLIC/USER FUNCTIONS                                                */
/************************************************************************/

/**
 * Function: serial_init
 * 
 * Initialises the serial module for use. This function must be called
 * prior to using any IO functions. This function initialises the USB
 * interface and creates the input and output serial streams.
 */
void serial_init() {
	usb_init();			  // Initialise USB serial
	stdin = &stdinout;
	stdout = &stdinout;
}

/**
 * Function: serial_ready
 * 
 * Returns: True when the USB interface is configured and ready 
 *          for use. Integer encodes a boolean value.
 */
uint8_t serial_ready() {
	return usb_configured();	// returns true if host has enumerated device
}

/**
 * Function: serial_available
 * 
 * Returns: True when there is a new character available on the 
 *          serial interface. Integer encodes a boolean value.
 */
uint8_t serial_available() {
	return usb_serial_available();
}
