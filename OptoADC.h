/*
 * OptoADC.h
 *
 * 
 *  Author: Caroline
 */ 


#ifndef OptoADC_H_
#define OptoADC_H_

#include "avr32\adcifb_101.h"

/************************************************************************/
/* Initialize the ADC interface
/* NOTE: Please ensure AVREF is connected correctly!
/*
/* Parameters:
/* volatile avr32_adcifb_t *adc - base address of ADC
/************************************************************************/
void adc_init(volatile avr32_adcifb_t *adc);

/************************************************************************/
/* Start conversion                                                     
/*
/* Parameters:
/* volatile avr32_adcifb_1 *adc - base address of ADC
/************************************************************************/
void adc_start(volatile avr32_adcifb_t *adc);

/************************************************************************/
/* Enable an ADC channel                                                
/*
/* Parameters:
/* volatile avr32_adcifb_t *adc - base address of ADC
/* unsigned short channel - ADC channel number
/************************************************************************/
void adc_enable(volatile avr32_adcifb_t *adc, unsigned short channel);

/************************************************************************/
/* Disable an ADC channel
/*
/* Parameters:
/* volatile avr32_adcifb_t *adc - base address of ADC
/* unsigned short channel - ADC channel number
/************************************************************************/
void adc_disable(volatile avr32_adcifb_t *adc, unsigned short channel);

/************************************************************************/
/* Check conversion status of ADC channel
/*
/* Parameters:
/* volatile avr32_adcifb_t *adc - base address of ADC
/*
/* Returns:
/* 0 for conversion in progress; 1 for ADC ready
/************************************************************************/
unsigned char conversion_status(volatile avr32_adcifb_t *adc);

#endif /* INCFILE1_H_ */