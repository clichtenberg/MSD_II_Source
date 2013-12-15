/*
 * OptoSPI.h
 *
 * Created: 11/19/2013 9:03:54 PM
 *  Author: Caroline
 */ 


#ifndef OPTOSPI_H_
#define OPTOSPI_H_

#include <avr32/io.h>
#include <stdint.h>

#include "avr32\uc3l064.h"
#include "avr32\spi_211.h"

/************************************************************************/
/* Initialize and configure SPI module
/*
/* Parameters:
/* volatile avr32_spi_t *spi - base address of SPI module
/************************************************************************/
void initSPI(volatile avr32_spi_t *spi);

/************************************************************************/
/* Initialize the SPI clock
/*
/* Parameters:
/* volatile avr32_spi_t *spi - base address of SPI module
/* unsigned pba_hz - peripheral bus speed (SPI on PBA)
/* unsigned baudrate - desired SPI baud rate (clock speed)
/************************************************************************/
void init_SPI_clk(volatile avr32_spi_t *spi, unsigned pba_hz, unsigned baudrate);

/************************************************************************/
/* Start SPI master transfer
/*
/* Parameters:
/* volatile avr32_spi_t *spi - base address of SPI module
/* uint16_t data - data to transfer
/************************************************************************/
void write_spi(volatile avr32_spi_t *spi, uint16_t data);

/************************************************************************/
/* Enable SPI module
/*
/* Parameters:
/* volatile avr32_spi_t *spi - base address of SPI module
/************************************************************************/
void enable_spi(volatile avr32_spi_t *spi);

/************************************************************************/
/* Disable SPI module
/*
/* Parameters:
/* volatile avr32_spi_t *spi - base address of SPI module
/************************************************************************/
void disable_spi(volatile avr32_spi_t *spi);

#endif /* OPTOSPI_H_ */