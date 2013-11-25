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

void initSPI(volatile avr32_spi_t *spi);

void init_SPI_clk(volatile avr32_spi_t *spi, unsigned pba_hz, unsigned baudrate);

void write_spi(volatile avr32_spi_t *spi, uint16_t data);

void enable_spi(volatile avr32_spi_t *spi);

void disable_spi(volatile avr32_spi_t *spi);

#endif /* OPTOSPI_H_ */