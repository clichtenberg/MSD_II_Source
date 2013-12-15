/*
 * OptoDMA.c
 *
 * 
 *  Author: Caroline
 */ 

#include "OptoDMA.h"
#include <stdio.h>

void init_adc_dma(unsigned int channel, volatile avr32_pdca_channel_t *dma, volatile uint16_t *adc_buffer){
	dma = &AVR32_PDCA.channel[channel];
	
	if(channel == AVR32_PDCA_PID_ADC_RX){
		// ADC
		dma->mar = (void*)adc_buffer;
		dma->TCR.tcv = 6; // change to 6 later
		dma->PSR.pid = AVR32_PDCA_PID_ADC_RX;
		dma->marr = (void*)adc_buffer;
		dma->TCRR.tcrv = 6; // change to 6 later
		dma->MR.size = AVR32_PDCA_SIZE_HALF_WORD; // Halfword
		dma->MR.etrig = 0;
		dma->MR.ring  = 1; // Ring buffer mode for continuous ADC sampling
	}
	else{
		return;
	}
}

void enable_dma(unsigned int channel, volatile avr32_pdca_channel_t *dma){
	dma = &AVR32_PDCA.channel[channel];
	
	dma->CR.ten = 1;
}

void disable_dma(unsigned int channel, volatile avr32_pdca_channel_t *dma){
	dma = &AVR32_PDCA.channel[channel];
	
	dma->CR.tdis = 1;
}

unsigned int dma_transfer_status(unsigned int channel, volatile avr32_pdca_channel_t *dma){
	dma = &AVR32_PDCA.channel[channel];
	
	// Transfer is complete if TCR and TCRR are both 0
	if((dma->TCR.tcv == 0) && (dma->TCRR.tcrv == 0)){
		// transfer complete
		return 1;
	}
	// transfer incomplete
	return 0;
}