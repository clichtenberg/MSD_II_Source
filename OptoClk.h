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

/************************************************************************/
/* Configure, initialize, and start RC clock (120 MHz). Clocks are
/* managed by system control interface and power management
/*
/* Parameters:
/* volatile avr32_scif_t *clk - base address of System Control Interface
/* volatile avr32_pm_t *pm_set - base address of Power Management
/************************************************************************/
void initClock(volatile avr32_scif_t *clk, volatile avr32_pm_t *pm_set);

#endif /* OPTOCLK_H_ */