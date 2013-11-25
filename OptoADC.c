/*
 * OptoADC.c
 *
 * 
 *  Author: Caroline
 */ 

#include "OptoADC.h"
#include <stdio.h>


void adc_init(volatile avr32_adcifb_t *adc){
	
	if(adc == NULL){
		return;
	}
	
	adc->CR.en = 1; // enables adc interface
	adc->ACR.res = AVR32_ADCIFB_ACR_RES_10BIT;
	// sampling frequency is now 460kSPS/4 = 115 kSPS (kHz)
	// Not setting prescale since we need ADC clock to run as fast as possible
	adc->ACR.shtim = 0xF; // sample and hold time
	adc->ACR.startup = 0x1F; // startup time in steps of 8 (need ~ 11 uS)
	adc->ACR.sleep = 0; // disable sleep mode
	adc->TRGR.trgmod = 0; // software trigger only
}


void adc_start(volatile avr32_adcifb_t *adc){
	
	if(adc == NULL){
		return;
	}
	adc->CR.start = 1;
}


void adc_enable(volatile avr32_adcifb_t *adc, unsigned short channel){
	
	if(adc == NULL){
		return;
	}
	if(channel > 0x08){
		return;
	}
	
	adc->cher |= (1 << channel);
}

void adc_disable(volatile avr32_adcifb_t *adc, unsigned short channel){
	
	if(adc == NULL){
		return;
	}
	if(channel > 0x08){
		return;
	}
	
	if(adc->chsr & (1 <<  channel)){
		adc->chdr |= (1 << channel);
	}	
}

unsigned char conversion_status(volatile avr32_adcifb_t *adc){
	
	if(adc == NULL){
		return -1;
	}
	
	if((adc->SR.ready == 0x00) && (adc->SR.busy == 0x01)){
		// conversion still in progress
		return 0;
	}
	else if((adc->SR.ready == 0x01) && (adc->SR.busy == 0x00)){
		// conversion done; ADCIFB ready for next conversion
		return 1;
	}
	//default in case of fall through
	return 0;
}