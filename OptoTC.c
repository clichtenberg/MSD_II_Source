/*
 * OptoTC.c
 *
 * Created: 11/13/2013 9:30:52 PM
 *  Author: Caroline
 */ 

#include "OptoTC.h"

void initTC(unsigned int channel, volatile avr32_tc_channel_t *tc){
	
	//tc = &AVR32_TC0.channel[channel];
	
	// We need a trigger every 0.021ms or 0.000021 s
	// PBA = 30 MHz
	// RC value = (PBA_f/n)*0.000021 and must be an integer
	// Thus, it makes sense to divide PBA by 2 so RC = 315
	tc->cmr = (AVR32_TC_NONE << AVR32_TC_BSWTRG_OFFSET|
								AVR32_TC_NONE << AVR32_TC_BEEVT_OFFSET|
								AVR32_TC_NONE << AVR32_TC_BCPB_OFFSET|
								AVR32_TC_NONE << AVR32_TC_BCPC_OFFSET|
								AVR32_TC_NONE << AVR32_TC_ASWTRG_OFFSET|
								AVR32_TC_NONE << AVR32_TC_AEEVT_OFFSET|
								AVR32_TC_NONE << AVR32_TC_ACPA_OFFSET|
								AVR32_TC_NONE << AVR32_TC_ACPC_OFFSET|
								1 << AVR32_TC_WAVE_OFFSET|
								AVR32_TC_WAVSEL_UP_AUTO << AVR32_TC_WAVSEL_OFFSET|
								0 << AVR32_TC_ENETRG_OFFSET|
								0 << AVR32_TC_EEVT_OFFSET|
								AVR32_TC_EEVTEDG_NO_EDGE << AVR32_TC_EEVTEDG_OFFSET|
								0 << AVR32_TC_CPCDIS_OFFSET|
								0 << AVR32_TC_CPCSTOP_OFFSET|
								0 << AVR32_TC_BURST_OFFSET|
								0 << AVR32_TC_CLKI_OFFSET|
								AVR32_TC_TCCLKS_TIMER_CLOCK2 << AVR32_TC_TCCLKS_OFFSET); // PBA_CLK/2
}

void initTC_interrupts(unsigned int channel, volatile avr32_tc_channel_t *tc){
	uint8_t temp;
	Disable_global_interrupt();
	
	// Enable interrupts
	tc->ier = (0 << AVR32_TC_ETRGS_OFFSET|
								0 << AVR32_TC_LDRBS_OFFSET|
								0 << AVR32_TC_LDRAS_OFFSET|
								1 << AVR32_TC_CPCS_OFFSET|
								0 << AVR32_TC_CPBS_OFFSET|
								0 << AVR32_TC_CPAS_OFFSET|
								0 << AVR32_TC_LOVRS_OFFSET|
								0 << AVR32_TC_COVFS_OFFSET);
								
	// Disable undesired interrupts
	tc->idr = (1 << AVR32_TC_ETRGS_OFFSET|
								1 << AVR32_TC_LDRBS_OFFSET|
								1 << AVR32_TC_LDRAS_OFFSET|
								0 << AVR32_TC_CPCS_OFFSET|
								1 << AVR32_TC_CPBS_OFFSET|
								1 << AVR32_TC_CPAS_OFFSET|
								1 << AVR32_TC_LOVRS_OFFSET|
								1 << AVR32_TC_COVFS_OFFSET);
	Enable_global_interrupt();
	temp = tc->sr;
}


void set_TC_RC(unsigned int channel, volatile avr32_tc_channel_t *tc){
	
	//tc = &AVR32_TC0.channel[channel];
	
	// We need a trigger every 0.021ms or 0.000021 s
	// PBA = 30 MHz
	// RC value = (PBA_f/n)*0.000021 and must be an integer
	// Thus, it makes sense to divide PBA by 2 so RC = 315
	
	//tc->rc = Wr_bitfield(tc->rc, AVR32_TC_RC_MASK, 315);
	tc->rc = (156 & AVR32_TC_RC_MASK) << AVR32_TC_RC_OFFSET;
}

void start_TC(unsigned int channel, volatile avr32_tc_channel_t *tc){
	
	//tc = &AVR32_TC0.channel[channel];
	tc->ccr = (1 << AVR32_TC_SWTRG_OFFSET|1 << AVR32_TC_CLKEN_OFFSET);
}