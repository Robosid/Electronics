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
 * buffer.h - EGB240DVR Library, Circular buffer module header
 *
 * Implements a circular buffer in RAM to temporarily store audio samples
 * when reading/writing to flash memory (SD card).
 *
 * Version: v1.0
 *    Date: 05/29/2017
 *  Modified By: Sid
 *  E-mail: robo_sid@yahoo.co.uk
 */ 

#ifndef BUFFER_H_
#define BUFFER_H_

// Initialises the buffer for first use. 
// Users must supply pointers to callback function implementation.
void buffer_init(void (*pFuncPageFull)(void), void (*pFuncPageEmpty)(void));	

void buffer_reset();				// Resets read/write pointers to top of buffer
void buffer_queue(uint8_t word);	// Writes a sample to the buffer and advances the write pointer
uint8_t buffer_dequeue();			// Reads a sample from the buffer and advances the read pointer
uint8_t* buffer_readPage();			// Allows user code to read a full page from the buffer
uint8_t* buffer_writePage();		// Allows user code to write a full page to the buffer

#endif /* BUFFER_H_ */