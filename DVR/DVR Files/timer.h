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