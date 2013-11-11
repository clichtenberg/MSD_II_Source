/*
 * OptoTWIM.c
 *
 * 
 *  Author: Caroline
 */ 

#include "OptoTWIM.h"

uint8_t init_twim(volatile avr32_twim_t *twim, uint32_t twim_speed, uint32_t twim_addr, uint32_t pb_freq){
	uint8_t r = 1;
	// Configure TWIM registers
	twim->CR.men = 1; // Master transfer enable
	twim->CR.swrst = 1; // reset the TWIM interface	
	twim->CR.smdis = 1;
	
	// Clear SCR
	twim->scr = 0xFFFFFFFF;
	
	// Set the TWIM speed
	set_twim_speed(twim, twim_speed,pb_freq);
	
	// Test that the chip responds
	if(!twim_ping_chip(twim, twim_addr)){
		// Chip response failed
		if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
			// Was it an address NAK or SDA bus error?
			r=0;
		}
		// Other error
		r=0;
	}
	
	// Initialization successful;
	return r;
}

void set_twim_speed(volatile avr32_twim_t *twim, uint32_t twim_speed, uint32_t pb_f){
	
	// clock configuration
	uint8_t cwgr_exp = 0; // clock divider 
	uint32_t cwgr_prescal = (pb_f/twim_speed/2); // prescaled value for TWCK
	
	// the TWCK must fit into an 8 bit field
	// the clock divider must fit in a 3 bit field
	while((cwgr_prescal > 0xFF) && (cwgr_exp <= 0x07)){
		// the clock divider can be increased
		// TWCK must be reduced by a factor of 2
		cwgr_exp++;
		cwgr_prescal /= 2;
	}
	
	// Set the clock settings for the desired TWCK
	twim->cwgr = (((cwgr_prescal/2) << AVR32_TWIM_CWGR_LOW_OFFSET)|((cwgr_prescal- cwgr_prescal/2) << AVR32_TWIM_CWGR_HIGH_OFFSET)
	|(cwgr_exp << AVR32_TWIM_CWGR_EXP_OFFSET)|(0 << AVR32_TWIM_CWGR_DATA_OFFSET)|(cwgr_prescal << AVR32_TWIM_CWGR_STASTO_OFFSET));
}

uint8_t twim_ping_chip(volatile avr32_twim_t *twim, uint32_t addr){
	
	uint8_t test_data = 0;
	
	if(twim_write(twim, test_data, 0, addr)){
		// TWIM write successful
		return 1;
	}
	return 0; // TWIM write failed
}

uint8_t twim_write(volatile avr32_twim_t *twim, uint8_t buff_data, uint32_t tx_bytes, uint32_t twim_addr){
	// Clear THR
	twim->thr = 0x00000000;
	twim->scr = 0xFFFFFFFF;
	twim->CR.mdis = 1;
	
	// Set data buffer
	twim->thr = buff_data << AVR32_TWIM_THR_TXDATA_OFFSET;
	
	// Set command register to start transfer
	twim->cmdr = ((twim_addr << AVR32_TWIM_CMDR_SADR_OFFSET)|(tx_bytes << AVR32_TWIM_CMDR_NBYTES_OFFSET)
					|(1 << AVR32_TWIM_CMDR_VALID_OFFSET)|(1 << AVR32_TWIM_CMDR_START_OFFSET)|(1 << AVR32_TWIM_CMDR_STOP_OFFSET));
	
	// Enable master to start transfer
	twim->CR.men = 1;
	
	// Wait for transfer to finish
	while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK));
	
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// Transfer failed due to no ACK received or state issue in SDA bus
		if(twim->sr & AVR32_TWIM_SR_DNAK_MASK){
			return 0;
		}
		else if(twim->sr & AVR32_TWIM_SR_ARBLST_MASK){
			return 0;
		}
		else if(twim->sr & AVR32_TWIM_SR_ANAK_MASK){
			return 0;
		}
		return 0;
	}
	
	// Transfer successful
	return 1;
}

uint8_t dac_call_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr){
	
	uint8_t twim_buffer;
	
	twim_buffer = 0;
	while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
	twim_write(twim, twim_buffer, 1, twim_addr);
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}

uint8_t dac_wakeup_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr){
	
	uint8_t twim_buffer;
	
	twim_buffer = 9;
	while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
	twim_write(twim, twim_buffer, 1, twim_addr);
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}

uint8_t dac_software_update(volatile avr32_twim_t *twim, uint32_t twim_addr){
	
	uint8_t twim_buffer;
	
	twim_buffer = 8;
	while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
	twim_write(twim, twim_buffer, 1, twim_addr);
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}

uint8_t dac_reset_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr){
	
	uint8_t twim_buffer;
	
	twim_buffer = 6;
	while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
	twim_write(twim, twim_buffer, 1, twim_addr);
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}

uint8_t dac_write_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr){
	
	uint8_t twim_buffer;
	
	// binary: 01011000
	// Where: 010 = cmd code (see DAC datasheet)
	// 11 = write code
	// 00 = channel A select
	// 0 = UDAC'
	twim_buffer = 88;
	while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
	twim_write(twim, twim_buffer, 1, twim_addr);
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}