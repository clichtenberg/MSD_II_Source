/*
 * OptoClk.h
 *
 * Created: 11/2/2013 10:20:34 PM
 *  Author: Caroline
 */ 


#ifndef OPTOCLK_H_
#define OPTOCLK_H_

#include "avr32\uc3l064.h"
#include "avr32\pm_411.h"
#include "avr32\scif_102.h"
#include "avr32\flashcdw_102.h"

#include <stdint.h>

void initClock(volatile avr32_scif_t *clk, volatile avr32_pm_t *pm_set);

uint8_t delay(unsigned int time);

#endif /* OPTOCLK_H_ */