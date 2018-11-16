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