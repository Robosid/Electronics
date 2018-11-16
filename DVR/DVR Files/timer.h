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
 * timer.h - EGB240DVR Library, Timer module header
 *
 * Configures Timer0 to generate regular interrupts for sampling 
 * and other timing purposes.
 *
 * Version: v1.0
  *    Date: 05/29/2017
  *  Modified By: Sid
  *  E-mail: robo_sid@yahoo.co.uk
 */ 

#ifndef TIMER_H_
#define TIMER_H_

// Defines for timer intervals
#define TIMER_INTERVAL_FATFS	156		// 10 ms interval
#define TIMER_INTERVAL_LED		7813	// 500 ms interval

void timer_init();	// Initialise and start Timer0

#endif /* TIMER_H_ */