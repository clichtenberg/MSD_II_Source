/*
 * OptoTWIM.h
 *
 * 
 *  Author: Caroline
 *
 * Description: Header file for the TWI for interfacing with the
 * MCP4728 (12-bit DAC)
 */ 


#ifndef OPTOTWIM_H_
#define OPTOTWIM_H_

#include <avr32/io.h>
#include <stdint.h>

#include "avr32\twim_101.h"

/************************************************************************/
/* Enable and configure the TWI
/*
/* Parameters:
/* avr32_twim_t *twi - base address of TWIM                                                                       
/************************************************************************/

uint8_t init_twim(volatile avr32_twim_t *twim, uint32_t twim_speed, uint32_t twim_addr, uint32_t pb_freq);

void set_twim_speed(volatile avr32_twim_t *twim, uint32_t twim_speed, uint32_t pb_f);

uint8_t twim_ping_chip(volatile avr32_twim_t *twim, uint32_t addr);

/************************************************************************/
/* TWI Write
/*
/* Parameters:
/* avr32_twim_t *twi - base address of TWIM
/* uint16_t buff_data
/* uint32_t twim_addr
/* uint8_t dac_cmd : Command for DAC communication
/*
/* SUPPORTED COMMANDS:
/* 0 - Ping DAC (verify DAC is connected to microcontroller)
/* 1 - Wakeup DAC
/* 2 - Software Update (updates all channels)
/* 3 - Reset DAC
/* 4 - Write to DAC
/************************************************************************/

uint8_t twim_write(volatile avr32_twim_t *twim, uint16_t buff_data, uint32_t twim_addr, uint8_t dac_cmd);

uint8_t dac_wakeup_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr);

uint8_t dac_software_update(volatile avr32_twim_t *twim, uint32_t twim_addr);

uint8_t dac_reset_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr);

uint8_t dac_write_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr, uint16_t twim_buffer);

#endif /* OPTOTWIM_H_ */