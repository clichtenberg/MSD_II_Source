/*
 * OptoGPIOConfig.h
 *
 * 
 *  Author: Caroline
 */ 


#ifndef OPTOGPIOCONFIG_H_
#define OPTOGPIOCONFIG_H_

#include "avr32\gpio_211.h"

/************************************************************************/
/* Enable module for a given GPIO pin
/*
/* Parameters:
/* unsigned int pin_num - pin number
/* unsigned int pin_mod - module to enable
/************************************************************************/
void assign_gpio_module(unsigned int pin_num, unsigned int pin_mod);

/************************************************************************/
/* Enable GPIO functionality for a given GPIO pin (useful for debugging)
/*
/* Parameters:
/* unsigned int pin_num - pin number
/************************************************************************/
void enable_gpio_pin(unsigned int pin_num);

#endif /* OPTOGPIOCONFIG_H_ */