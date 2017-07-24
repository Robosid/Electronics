/**
 * serial.h - EGB240DVR Library, Serial interface header
 *
 * Provides a serial interface via the USB port.
 *
 * Version: v1.0
 *    Date: 05/29/2017
 *  Modified By: Sid
 *  E-mail: robo_sid@yahoo.co.uk
 */ 

#ifndef SERIAL_H_
#define SERIAL_H_

void serial_init();			// Initialises the serial module for use.
uint8_t serial_ready();		// Returns true if the serial interface is ready for use.
uint8_t serial_available(); // Returns true if characters are available on the serial interface.

#endif /* SERIAL_H_ */