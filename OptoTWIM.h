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
/* NOTE:
/* To use TWIM interface, you'll need to add these lines back into
/* main.c:
/* volatile avr32_twim_t *twi = &AVR32_TWIM1;
/* enable_gpio_pin(AVR32_PIN_PA17);
/* enable_gpio_pin(AVR32_PIN_PA05);
/* assign_gpio_module(AVR32_TWIMS1_TWCK_0_0_PIN, AVR32_TWIMS1_TWCK_0_0_FUNCTION);
/* assign_gpio_module(AVR32_TWIMS1_TWD_0_PIN, AVR32_TWIMS1_TWD_0_FUNCTION);
/************************************************************************/

/************************************************************************/
/* Enable and configure the TWI
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t twim_speed - clock speed for TWIM
/* uint32_t twim_addr - slave address to write to
/* uint32_t pb_freq - peripheral bus speed
/*
/* Returns:
/* 1 for success; 0 otherwise                                                           
/************************************************************************/
uint8_t init_twim(volatile avr32_twim_t *twim, uint32_t twim_speed, uint32_t twim_addr, uint32_t pb_freq);

/************************************************************************/
/* Enable and configure the TWI
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t twim_speed - clock speed for TWIM
/* uint32_t pb_f - peripheral bus speed
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
void set_twim_speed(volatile avr32_twim_t *twim, uint32_t twim_speed, uint32_t pb_f);

/************************************************************************/
/* Enable and configure the TWI
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t addr - slave address to write to
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
uint8_t twim_ping_chip(volatile avr32_twim_t *twim, uint32_t addr);

/************************************************************************/
/* TWI Write
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint16_t buff_data - data to transfer
/* uint32_t twim_addr - slave address to write to
/* uint8_t dac_cmd - command for DAC communication
/*
/* SUPPORTED COMMANDS for MCP4728:
/* 0 - Ping DAC (verify DAC is connected to microcontroller)
/* 1 - Wakeup DAC
/* 2 - Software Update (updates all channels)
/* 3 - Reset DAC
/* 4 - Write to DAC
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
uint8_t twim_write(volatile avr32_twim_t *twim, uint16_t buff_data, uint32_t twim_addr, uint8_t dac_cmd);

/************************************************************************/
/* MCP4728 Specific Commands
/************************************************************************/

/************************************************************************/
/* Remove DAC from idle state
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t twim_addr - slave address to write to
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
uint8_t dac_wakeup_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr);

/************************************************************************/
/* Update all DAC outputs with current content of DAC memory
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t twim_addr - slave address to write to
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
uint8_t dac_software_update(volatile avr32_twim_t *twim, uint32_t twim_addr);

/************************************************************************/
/* Reset current DAC configuration
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t twim_addr - slave address to write to
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
uint8_t dac_reset_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr);

/************************************************************************/
/* Write to DAC memory
/*
/* Parameters:
/* avr32_twim_t *twim - base address of TWIM
/* uint32_t twim_addr - slave address to write to
/* uint16_t twim_buffer - data to write to DAC memory
/*
/* Returns:
/* 1 for success; 0 otherwise
/************************************************************************/
uint8_t dac_write_cmd(volatile avr32_twim_t *twim, uint32_t twim_addr, uint16_t twim_buffer);

#endif /* OPTOTWIM_H_ */