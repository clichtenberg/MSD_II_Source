/*
 * OptoFilter.h
 *
 * Created: 11/11/2013 5:58:14 PM
 *  Author: Caroline
 */ 


#ifndef OPTOFILTER_H_
#define OPTOFILTER_H_

#include <stdio.h>
#include <stdint.h>
#include "dsp.h"

/************************************************************************/
/* Bandpass filtering for each of the 6 guitar strings
/*
/* Parameters:
/* uint16_t *signal_data - address of stored ADC samples
/* uint8_t string_num - corresponding guitar string number
/*
/* Returns:
/* Filtered data for the corresponding string number
/************************************************************************/
uint16_t fir_bpf(uint16_t *signal_data, uint8_t string_num);

/************************************************************************/
/* Sum all six string data together
/*
/* Parameters:
/* uint16_t *signal_data - address of filtered samples
/*
/* Returns:
/* Summation of all string data
/************************************************************************/
uint16_t sum_signal(uint16_t *signal_data);


#endif /* OPTOFILTER_H_ */