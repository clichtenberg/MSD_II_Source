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
	
	if(twim_write(twim, test_data, addr, 0)){
		// TWIM write successful
		return 1;
	}
	return 0; // TWIM write failed
}

uint8_t twim_write(volatile avr32_twim_t *twim, uint16_t buff_data, uint32_t twim_addr, uint8_t dac_cmd){
	
	// Command for DAC write
	// binary: 01011000
	// Where: 010 = cmd code (see DAC datasheet)
	// 11 = write code
	// 00 = channel A select
	// 0 = UDAC'
	uint8_t cmd_dac_wr = 88;
	
	// Clear THR
	twim->thr = 0x00000000;
	twim->scr = 0xFFFFFFFF;
	twim->CR.mdis = 1;
	
	// TODO: ADD dac_cmd support (see DAC data sheet for transmission formats)
	switch(dac_cmd){
		
		// Ping DAC
		case 0:
			// Set data buffer
			twim->thr = buff_data << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Set command register to start transfer
			twim->cmdr = ((twim_addr << AVR32_TWIM_CMDR_SADR_OFFSET)|(0 << AVR32_TWIM_CMDR_NBYTES_OFFSET)
			|(1 << AVR32_TWIM_CMDR_VALID_OFFSET)|(1 << AVR32_TWIM_CMDR_START_OFFSET)|(1 << AVR32_TWIM_CMDR_STOP_OFFSET));
			
			// Enable master to start transfer
			twim->CR.men = 1;
			
			// Wait for transfer to finish
			while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK));
			
			break;
			
		// Wakeup DAC
		case 1:
			// Set data buffer
			twim->thr = 0 << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Set command register to start transfer
			twim->cmdr = ((twim_addr << AVR32_TWIM_CMDR_SADR_OFFSET)|(2 << AVR32_TWIM_CMDR_NBYTES_OFFSET)
			|(1 << AVR32_TWIM_CMDR_VALID_OFFSET)|(1 << AVR32_TWIM_CMDR_START_OFFSET)|(1 << AVR32_TWIM_CMDR_STOP_OFFSET));
			
			// Enable master to start transfer
			twim->CR.men = 1;
			// Wait to load next byte
			while(!(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
			twim->thr = buff_data << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Wait for transfer to finish
			while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK));
			
			break;
			
		// Software Update
		case 2:
			// Set data buffer
			twim->thr = 0 << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Set command register to start transfer
			twim->cmdr = ((twim_addr << AVR32_TWIM_CMDR_SADR_OFFSET)|(2 << AVR32_TWIM_CMDR_NBYTES_OFFSET)
			|(1 << AVR32_TWIM_CMDR_VALID_OFFSET)|(1 << AVR32_TWIM_CMDR_START_OFFSET)|(1 << AVR32_TWIM_CMDR_STOP_OFFSET));
			
			// Enable master to start transfer
			twim->CR.men = 1;
			
			// Wait to load next byte
			while(!(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
			twim->thr = buff_data << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Wait for transfer to finish
			while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK));
			
			break;
			
		// Reset DAC
		case 3:
			// Set data buffer
			twim->thr = 0 << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Set command register to start transfer
			twim->cmdr = ((twim_addr << AVR32_TWIM_CMDR_SADR_OFFSET)|(2 << AVR32_TWIM_CMDR_NBYTES_OFFSET)
			|(1 << AVR32_TWIM_CMDR_VALID_OFFSET)|(1 << AVR32_TWIM_CMDR_START_OFFSET)|(1 << AVR32_TWIM_CMDR_STOP_OFFSET));
			
			// Enable master to start transfer
			twim->CR.men = 1;
			
			// Wait to load next byte
			while(!(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
			twim->thr = buff_data << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Wait for transfer to finish
			while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK));
			
			break;
			
		// Write to DAC
		case 4:
			// Set data buffer
			twim->thr = cmd_dac_wr << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Set command register to start transfer
			twim->cmdr = ((twim_addr << AVR32_TWIM_CMDR_SADR_OFFSET)|(3 << AVR32_TWIM_CMDR_NBYTES_OFFSET)
			|(1 << AVR32_TWIM_CMDR_VALID_OFFSET)|(1 << AVR32_TWIM_CMDR_START_OFFSET)|(1 << AVR32_TWIM_CMDR_STOP_OFFSET));
			
			// Enable master to start transfer
			twim->CR.men = 1;
			
			// Wait to load next byte (1st data byte)
			while(!(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
			twim->thr = ((uint8_t)((buff_data & 0x0F00) >> 8)) << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// 2nd data byte
			while(!(twim->sr & AVR32_TWIM_SR_TXRDY_MASK));
			twim->thr = ((uint8_t)(buff_data & 0x00FF)) << AVR32_TWIM_THR_TXDATA_OFFSET;
			
			// Wait for transfer to finish
			while(!(twim->sr & AVR32_TWIM_SR_IDLE_MASK));
			
			break;
			
		// Invalid command
		default:
			return 0;
	}
	
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

uint8_t dac_wakeup_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr){
	
	uint8_t twim_buffer;
	twim_buffer = 9;
	
	// Clear status register
	twim->scr = 0xFFFFFFFF;
	
	// Write
	twim_write(twim, twim_buffer, twim_addr, 1);
	
	// Error checks
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
	
	// Clear status register
	twim->scr = 0xFFFFFFFF;
	
	// Write
	twim_write(twim, twim_buffer, twim_addr, 2);
	
	// Error checks
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
	
	// Clear status register
	twim->scr = 0xFFFFFFFF;
	
	// Write
	twim_write(twim, twim_buffer, twim_addr, 3);
	
	// Error checks
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}

uint8_t dac_write_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr, uint16_t twim_buffer){
	
	// Clear status register
	twim->scr = 0xFFFFFFFF;
	
	// Write
	twim_write(twim, twim_buffer, twim_addr, 4);
	
	// Error checks
	if((twim->sr & AVR32_TWIM_SR_ARBLST_MASK) || (twim->sr & AVR32_TWIM_SR_DNAK_MASK) || (twim->sr & AVR32_TWIM_SR_ANAK_MASK)){
		// uh oh problem
		return 0;
	}
	// success
	return 1;
}