/*
 * OptoClk.c
 *
 * Created: 11/2/2013 10:20:19 PM
 *  Author: Caroline
 */ 

#include "OptoClk.h"

#include <stdint.h>

void initClock(volatile avr32_scif_t *clk, volatile avr32_pm_t *pm_set){

	// Desired CPU and PBA freq = 30 MHz
	// RCMHz freq = 120 MHz
	
	// Enable RC120M clock
	AVR32_SCIF.unlock = ((AVR32_SCIF_UNLOCK_KEY_VALUE << AVR32_SCIF_UNLOCK_KEY_OFFSET)|AVR32_SCIF_RC120MCR);
	AVR32_SCIF.rc120mcr = AVR32_SCIF_RC120MCR_EN_MASK;
	
	while(!(pm_set->sr & AVR32_PM_SR_CKRDY_MASK)); // clock sel must not be written while ckrdy is 0
	
	uint8_t div_val = 1; // need to divide RCMHz osc freq to get 30 MHz
	uint8_t div_pba = 1;
	uint8_t div_pbb = 1;
	
	while(!(pm_set->sr & AVR32_PM_SR_CKRDY_MASK));
	// Set division ratio
	pm_set->unlock = ((AVR32_PM_UNLOCK_KEY_VALUE << AVR32_PM_UNLOCK_KEY_OFFSET)|AVR32_PM_CPUSEL);
	pm_set->cpusel = ((1 << AVR32_PM_CPUSEL_CPUDIV_OFFSET)|(div_val << AVR32_PM_CPUSEL_CPUSEL_OFFSET));
	
	while(!(pm_set->sr & AVR32_PM_SR_CKRDY_MASK));
	
	if(pm_set->sr & AVR32_PM_SR_CFO_MASK){
		return;
	}
	
	pm_set->unlock = ((AVR32_PM_UNLOCK_KEY_VALUE << AVR32_PM_UNLOCK_KEY_OFFSET)|AVR32_PM_PBASEL);
	pm_set->pbasel = ((1 << AVR32_PM_PBASEL_PBDIV_OFFSET)|(div_pba << AVR32_PM_PBASEL_PBSEL_OFFSET));
	
	while(!(pm_set->sr & AVR32_PM_SR_CKRDY_MASK));
	
	pm_set->unlock = ((AVR32_PM_UNLOCK_KEY_VALUE << AVR32_PM_UNLOCK_KEY_OFFSET)|AVR32_PM_PBBSEL);
	pm_set->pbbsel = ((1 << AVR32_PM_PBBSEL_PBDIV_OFFSET)|(div_pbb << AVR32_PM_PBBSEL_PBSEL_OFFSET));
	
	while(!(pm_set->sr & AVR32_PM_SR_CKRDY_MASK));
	
	pm_set->unlock = ((AVR32_PM_UNLOCK_KEY_VALUE << AVR32_PM_UNLOCK_KEY_OFFSET)|AVR32_PM_MCCTRL);
	pm_set->mcctrl = AVR32_PM_MCSEL_RC120M;
	
	while(!(pm_set->sr & AVR32_PM_SR_CKRDY_MASK));
}

uint8_t delay(unsigned int time){
	while(time != 0){
		--time;
	}
	return 1;
}