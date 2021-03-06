/*
 * OptoDMA.h
 *
 * 
 *  Author: Caroline
 */ 


#ifndef OPTODMA_H_
#define OPTODMA_H_

#include <stdint.h>

#include "avr32\uc3l064.h"
#include "avr32\pdca_122.h"

/************************************************************************/
/* Initialize DMA for ADC
/*
/* Parameters:
/* volatile avr32_pdca_channel_t *dma - base address of DMA
/* unsigned int channel - DMA channel number
/* volatile uint16_t *adc_buffer - address to store ADC samples
/************************************************************************/
void init_adc_dma(unsigned int channel, volatile avr32_pdca_channel_t *dma, volatile uint16_t *adc_buffer);

/************************************************************************/
/* Enable DMA for a given channel
/*
/* Parameters:
/* volatile avr32_pdca_channel_t *dma - base address of DMA
/* unsigned int channel - DMA channel number
/************************************************************************/
void enable_dma(unsigned int channel, volatile avr32_pdca_channel_t *dma);

/************************************************************************/
/* Disable DMA for a given channel
/*
/* Parameters:
/* volatile avr32_pdca_channel_t *dma - base address of DMA
/* unsigned int channel - DMA channel number
/************************************************************************/
void disable_dma(unsigned int channel, volatile avr32_pdca_channel_t *dma);

/************************************************************************/
/* Get the DMA channel transfer status
/*
/* Parameters:
/* volatile avr32_pdca_channel_t *dma - base address of DMA
/* unsigned int channel - DMA channel number
/*
/* Returns:
/* 0 if transfer incomplete; 1 if transfer complete
/************************************************************************/
unsigned int dma_transfer_status(unsigned int channel, volatile avr32_pdca_channel_t *dma);

#endif /* OPTODMA_H_ */