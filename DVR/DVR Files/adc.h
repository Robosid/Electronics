/**
 * adc.h - EGB240DVR Library, ADC module header
 *
 * Version: v1.0
 *    Date: 05/29/2017
 *  Modified By: Sid
 *  E-mail: robo_sid@yahoo.co.uk
 */ 

#ifndef ADC_H_
#define ADC_H_

void adc_init();	// Initialises ADC
void adc_start();	// Enables ADC to start conversions (triggered by Timer0 CMPA)
void adc_stop();	// Disables ADC conversions

#endif /* ADC_H_ */