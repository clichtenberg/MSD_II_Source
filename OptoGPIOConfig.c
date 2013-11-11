/*
 * OptoGPIOConfig.c
 *
 * 
 *  Author: Caroline
 */ 

#include "avr32\uc3l064.h"
#include "avr32\gpio_211.h"

void assign_gpio_module(unsigned int pin_num, unsigned int pin_mod){
	volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin_num>>5];
	
	switch(pin_mod){
		// pmr - peripheral mux register
		// to set peripheral function, we need to write
		// to the correct PMRn indices
		case 0:
			// Function = A
			gpio_port->pmr0c = (1<<(pin_num & 0x1F));
			gpio_port->pmr1c = (1<<(pin_num & 0x1F));
			gpio_port->pmr2c = (1<<(pin_num & 0x1F));
			break;
		case 1:
			// Function = B
			gpio_port->pmr0s = (1<<(pin_num & 0x1F));
			gpio_port->pmr1c = (1<<(pin_num & 0x1F));
			gpio_port->pmr2c = (1<<(pin_num & 0x1F));
			break;
		case 2:
			// Function = C
			gpio_port->pmr0c = (1<<(pin_num & 0x1F));
			gpio_port->pmr1s = (1<<(pin_num & 0x1F));
			gpio_port->pmr2c = (1<<(pin_num & 0x1F));
			break;
		case 3:
			// Function = D
			gpio_port->pmr0s = (1<<(pin_num & 0x1F));
			gpio_port->pmr1s = (1<<(pin_num & 0x1F));
			gpio_port->pmr2c = (1<<(pin_num & 0x1F));
			break;
		case 4:
			// Function = E
			gpio_port->pmr0c = (1<<(pin_num & 0x1F));
			gpio_port->pmr1c = (1<<(pin_num & 0x1F));
			gpio_port->pmr2s = (1<<(pin_num & 0x1F));
		case 5:
			// Function = F
			gpio_port->pmr0s = (1<<(pin_num & 0x1F));
			gpio_port->pmr1c = (1<<(pin_num & 0x1F));
			gpio_port->pmr2s = (1<<(pin_num & 0x1F));
		case 6:
			// Function = G
			gpio_port->pmr0c = (1<<(pin_num & 0x1F));
			gpio_port->pmr1s = (1<<(pin_num & 0x1F));
			gpio_port->pmr2s = (1<<(pin_num & 0x1F));
		case 7:
			// Function = H
			gpio_port->pmr0s = (1<<(pin_num & 0x1F));
			gpio_port->pmr1s = (1<<(pin_num & 0x1F));
			gpio_port->pmr2s = (1<<(pin_num & 0x1F));
		default:
			// Invalid GPIO
			return;
	}
	// Disable GPIO function for pin to enable peripheral
	gpio_port->gperc = (1<<(pin_num & 0x1F));
}


void enable_gpio_pin(unsigned int pin_num){
	volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin_num>>5];
	
	gpio_port->oderc = (1 << (pin_num & 0x1F));
	gpio_port->gpers = (1 << (pin_num & 0x1F));
}