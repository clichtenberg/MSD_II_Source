/*
 * OptoTC.h
 *
 * Created: 11/13/2013 9:30:37 PM
 *  Author: Caroline
 */ 


#ifndef OPTOTC_H_
#define OPTOTC_H_

#include <stdint.h>

#include "intc.h"
#include "interrupt/interrupt_avr32.h"

#include "avr32\uc3l064.h"
#include "avr32\tc_2231.h"

void initTC(unsigned int channel, volatile avr32_tc_channel_t *tc);

void initTC_interrupts(unsigned int channel, volatile avr32_tc_channel_t *tc);

void set_TC_RC(unsigned int channel, volatile avr32_tc_channel_t *tc);

void start_TC(unsigned int channel, volatile avr32_tc_channel_t *tc);

#endif /* OPTOTC_H_ */