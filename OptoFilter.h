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

uint16_t fir_bpf(uint16_t *signal_data);

uint16_t sum_signal(uint16_t *signal_data);


#endif /* OPTOFILTER_H_ */