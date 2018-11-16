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