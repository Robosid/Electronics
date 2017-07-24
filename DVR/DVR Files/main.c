/**
 * main.c - EGB240 Digital Voice Recorder Skeleton Code
 *
 * This code provides a skeleton implementation of a digital voice 
 * recorder using the Teensy microcontroller and QUT TensyBOBv2 
 * development boards. This skeleton code demonstrates usage of
 * the EGB240DVR library, which provides functions for recording
 * audio samples from the ADC, storing samples temporarily in a 
 * circular buffer, and reading/writing samples to/from flash
 * memory on an SD card (using the FAT file system and WAVE file
 * format. 
 *
 * This skeleton code provides a recording implementation which 
 * samples CH0 of the ADC at 8-bit, 15.625kHz. Samples are stored 
 * in flash memory on an SD card in the WAVE file format. The 
 * filename is set to "EGB240.WAV". The SD card must be formatted 
 * with the FAT file system. Recorded WAVE files are playable on 
 * a computer.
 * 
 * LED4 on the TeensyBOBv2 is configured to flash as an 
 * indicator that the programme is running; a 1 Hz, 50 % duty
 * cycle flash should be observed under normal operation.
 *
 * A serial USB interface is provided as a secondary control and
 * debugging interface. Errors will be printed to this interface.
 *
 * Version: v2.0
 *    Date: 05/29/2017
 *  Modified by: Sid 
 *  E-mail: robo_sid@yahoo.co.uk
 */  

 /************************************************************************/
 /* INCLUDED LIBRARIES/HEADER FILES                                      */
 /************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

#include <stdlib.h>

#include "serial.h"
#include "timer.h"
#include "wave.h"
#include "buffer.h"
#include "adc.h"
#include "lib/fatfs/ff.h"
#include "lib/fatfs/diskio.h"

#define TOP 255

/************************************************************************/
/* ENUM DEFINITIONS                                                     */
/************************************************************************/
enum {
	DVR_STOPPED,
	DVR_RECORDING,
	DVR_PLAYING
};

/************************************************************************/
/* GLOBAL VARIABLES                                                     */
/************************************************************************/
volatile uint16_t countpage = 0;
volatile uint16_t pageCount = 0;	// Page counter - used to terminate recording
volatile uint16_t newPage = 0;	// Flag that indicates a new page is available for read/write
volatile uint8_t stop = 0;		// Flag that indicates playback/recording is complete
volatile uint8_t ticks = 0;
volatile uint8_t number  = 2;
uint8_t check = 0;
volatile uint8_t fast = 0;

//FATFS fs2;
//FIL file2;

/************************************************************************/
/* GLOBAL VARIABLES FOR DEBOUNCING ONLY                                 */
/************************************************************************/
volatile uint8_t push_button0;
volatile uint8_t push_button1;
volatile uint8_t push_button2;
volatile uint8_t push_button_stable=0;
uint8_t push_buttons = 0;
uint8_t PB3_val = 0;
uint8_t PB4_val = 0, prev_PB4_val = 0, PB4_edge = 0;

/************************************************************************/
/* FUNCTION PROTOTYPES                                                  */
/************************************************************************/
void pageFull();
void pageEmpty();
void PwM_start();
//void debounce();
//void debounce_init();
/************************************************************************/
/* INITIALISATION FUNCTIONS                                             */
/************************************************************************/

// Initialise PLL (required by USB serial interface, PWM)
void pll_init() {
	PLLFRQ = 0x6A; // PLL = 96 MHz, USB = 48 MHz, TIM4 = 64 MHz
}

// Configure system clock for 16 MHz
void clock_init() {
	CLKPR = 0x80;	// Prescaler change enable
	CLKPR = 0x00;	// Prescaler /1, 16 MHz
}

// Initialise DVR subsystems and enable interrupts
void init() {
		cli();			// Disable interrupts
		clock_init();	// Configure clocks
		pll_init();     // Configure PLL (used by Timer4 and USB serial)
		serial_init();	// Initialise USB serial interface (debug)
		timer_init();	// Initialise timer (used by FatFs library)
		buffer_init(pageFull, pageEmpty);  // Initialise circular buffer (must specify callback functions)
		adc_init();		// Initialise ADC
		sei();			// Enable interrupts
	    DDRF &= 0b10001111;    // Pushbuttons 1 to 3 - PORTF 6-4 as inputs
	    DDRD |= 0b11110000;		// Set PORTD 7-4 as outputs (LEDs)	
	
		// Must be called after interrupts are enabled
		wave_init();	// Initialise WAVE file interface
}

/************************************************************************/
/* CALLBACK FUNCTIONS FOR CIRCULAR BUFFER                               */
/************************************************************************/

// CALLED FROM BUFFER MODULE WHEN A PAGE IS FILLED WITH RECORDED SAMPLES
void pageFull() 
{
	if(!(--pageCount)) 
	{
		// If all pages have been filled
		adc_stop();		// Stop recording (disable new ADC conversions)
		stop = 1;		// Flag recording complete
	}

	else 
		newPage = 1;	// Flag new page is ready to write to SD card
}

// CALLED FROM BUFFER MODULE WHEN A NEW PAGE HAS BEEN EMPTIED
void pageEmpty() 
{
		if(!(--pageCount)) // If all pages have been read.
		stop = 1;		// Flag playback complete
	

	else 
		newPage = 1;	// Flag new page is ready to read to buffer.

}

// FOR STORING THE LAST PAGE IN CASE OF FORCE STOP, WHILE RECORDING.
/*
void write_file(uint16_t* lastpage, uint16_t c)
{   
	//lastpage = 500 - lastpage; 
   // uint16_t k = 500 - pageCount;
	f_mount(&fs2, "/", 1);
	uint16_t bw;
    f_open(&file2, "last_page.txt", FA_CREATE_ALWAYS | FA_WRITE);
	f_write(&file2,lastpage,c,&bw);
    f_close(&file2);
}

uint16_t read_file()
{        
	            uint16_t lastpage = 0;
	            //f_mount(&fs2, "/", 1);
			    f_open(&file2, "last_page.txt", FA_READ);
			    f_read(&file2,lastpage);
                f_close(&file2);
				return lastpage;
} 

uint16_t* ret()
{   uint16_t k = 500 - pageCount;
	uint16_t* r = &k;
	
	return r;
} 
*/

void debounce_init()
{
	push_button0;
	push_button1;
	push_button2;
	push_button_stable=0;
	push_buttons = 0;
	PB3_val = 0;
	PB4_val = 0, prev_PB4_val = 0, PB4_edge = 0;

}

void debounce()
{
	if (push_button_stable)
	push_buttons = push_button0;

	PB3_val = push_buttons & 0b01000000;
	PB4_val = push_buttons & 0b10000000;

	//reset if PB3 pressed
	//if (PB3_val != 0)
	//stop = 1;

	//find transitions of PB4 from 0->1
	PB4_edge = ((prev_PB4_val == 0) && (PB4_val != 0));

	//PORTD |= 0b00010000;  // turn LED1 on
	//PORTD &= 0b00011111;  // turn other LEDs off
		
	if (PB4_edge && (fast == 0))
		{fast = 1; number = 1; ticks = 0; PORTD |= 0b10000000; }
		
	else if (PB4_edge && (fast == 1))
		{fast = 0; number = 2; ticks = 0; PORTD &= 0b01111111;}

	prev_PB4_val = PB4_val;

}
/************************************************************************/
/* RECORD/PLAYBACK ROUTINES                                             */
/************************************************************************/

// Initiates a record cycle
void dvr_record() 
{  
	buffer_reset();		// Reset buffer state
	countpage = 0;
	pageCount = 900;	// Maximum record time - 30 sec
	newPage = 0;		// Clear new page flag
	
	wave_create();		// Create new wave file on the SD card
	adc_start();		// Begin sampling
	PORTD |= 0b01100000;
}

void dvr_play()
{  
	buffer_reset();
	//volatile uint16_t pageBreak = read_file();
   // if (page_break > 0)
    //	pageCount = page_break;
    
   // else
    	pageCount = countpage;
	    fast = 0;
	    number = 2;
	    ticks = 0;
    PORTD |= 0b00010000;
    wave_open();
	wave_read(buffer_writePage(), 1024);
	newPage = 0;
	PwM_start();
	debounce_init();
	debounce();

}

 void PwM_start()
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
	 TCCR4B = 0b00000100; 
	 TCCR4A = 0x21;   // bits 5 & 4 set clear on CMP, bit 0 enables PWM // TCCR4A FOR OCR4D
	 OCR4C = TOP;    // set top to 0xFF (255)
	 OCR4B = 128;   // initialize to DC duty cycle. Default DC = 50%. 
	 TIMSK4 = 0x04;	// Timer/Counter4 Overflow Interrupt Enable
	 TCNT4 = 0x00;  // reset timer


	 serial_init();	// Initialise USB serial interface (debug)

	 sei();
 }

 void PwM_stop(){
      check = 1;
	  	 TCNT4 = 0x00;
	     TCCR4A = 0x00;
    	 TIMSK4 = 0x00;
	     OCR4B = 0;

 }

	ISR(TIMER4_OVF_vect) {
	if (~TIFR4 & 0b00000100 )
	    ticks++;

		
	if (ticks == number)		
	{
		uint8_t fidgit = buffer_dequeue();		//dequeue here
		OCR4B = fidgit ;
		ticks = 0;
	}
	
	push_button2 = push_button1;
	push_button1 = push_button0;
	push_button0 = ~PINF;

	// If last 3 PB readings the same, consider this stable
	if ((push_button0 == push_button1) && (push_button0 == push_button2))
	push_button_stable = 1;
	else
	push_button_stable = 0;
	
	}//ISR

/************************************************************************/
/* MAIN LOOP (CODE ENTRY)                                               */
/************************************************************************/
int main(void) 
{
		
	uint8_t state = DVR_STOPPED;	// Start DVR in stopped state
	//uint16_t pageBreak = 0;
	//uint8_t push_buttons = 0;
	//uint8_t PB3_val = 0;
	//uint8_t PB4_val = 0, prev_PB4_val = 0, PB4_edge = 0;
	
	// Initialisation
	init();	
	debounce_init();
	// state machine
    for(;;) {
		 
		// Switch depending on state
		switch (state) 
		{   
			case DVR_STOPPED:
				PORTD |= 0b01000000;
				if (~PINF & 0b00010000) //S1-Initiate Playback
				{
			     	printf("Begin Playback...");	// Output status to console
			     	dvr_play(); //Initiate Playback 
					state = DVR_PLAYING;  // Transition to "recording" state
	                PORTD &= 0b10111111;
                }
                 
				if (~PINF & 0b00100000) 
				{
			     	printf("Start Recording...");	// Output status to console
					dvr_record();			// Initiate recording
					state = DVR_RECORDING;  // Transition to "recording" state
	                PORTD &= 0b10111111;
                }

				break;

			case DVR_RECORDING: 
				 if (~PINF & 0b01000000) //Stop button pressed
				 {
				 	//pageBreak = 500 - pageCount;
					//write_file(ret(),2);
					pageCount = 1;	// Finish recording last page
					
				 }
			
				// Write samples to SD card when buffer page is full
				if (newPage) 
				{   countpage++;
					newPage = 0;	// Acknowledge new page flag
					wave_write(buffer_readPage(), 512);
				} 
				else if (stop) 
				{
					// Stop is flagged when the last page has been recorded
					stop = 0;							// Acknowledge stop flag
					wave_write(buffer_readPage(), 512);	// Write final page
					wave_close();						// Finalise WAVE file 
					adc_stop();                         // Stop  ADC sampling
					printf("completed recording\n");    // Print status to console
					PORTD &= 0b11011111;
					while (~PINF & 0b00100000){
						printf("Please release record button ........ \n");
						continue;}
					state = DVR_STOPPED;				// Transition to stopped state6

					}

				break;

			case DVR_PLAYING:
                debounce(); 
                if (newPage) 
				{
					newPage = 0;	// Acknowledge new page flag
					wave_read(buffer_writePage(), 512);

				}
                else if (stop || (~PINF & 0b01000000)) 
				{
					// Stop is flagged when the last page has been played
					stop = 0;							// Acknowledge stop flag
					wave_close();						// Finalise WAVE file 
					PwM_stop();                         // Stop  PWM
					printf("completed recording\n");    // Print status to console
					PORTD &= 0b11101111;
					PORTD &= 0b01111111;
					state = DVR_STOPPED;				// Transition to stopped state
					fast = 0;
					number = 2;

				}
				
//				if ((~PINF & 0b10000000) && (fast == 0))
//					{fast = 1; number = 1; ticks = 0; PORTD |= 0b10000000; }
//				
//				else if ((~PINF & 0b10000000) && (fast == 1))
//				    {fast = 0; number = 2; ticks = 0; PORTD &= 0b01111111;}

				break;
			default:
				// Invalid state, return to valid idle state (stopped)
				printf("ERROR: State machine in main entered invalid state!\n");
				state = DVR_STOPPED;
			    PORTD |= 0b01000000;
				break;

		} // END switch(state)
	 // END for(;;)
	}
				

}

