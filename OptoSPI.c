/*
 * OptoSPI.c
 *
 * Created: 11/19/2013 9:04:11 PM
 *  Author: Caroline
 */ 

#include "OptoSPI.h"

void initSPI(volatile avr32_spi_t *spi){
	// set SPI in master mode
	// Reset SPI module
	spi->cr = (1 << AVR32_SPI_SWRST_OFFSET);
	
	// Set mode in master mode
	spi->mr = (1 << AVR32_SPI_MSTR_OFFSET |	1 << AVR32_SPI_MODFDIS_OFFSET 
				| 0 << AVR32_SPI_LLB_OFFSET | 0 << AVR32_SPI_PS_OFFSET
				| 14 << AVR32_SPI_PCS_OFFSET
				| 0 << AVR32_SPI_PCSDEC_OFFSET | 0 << AVR32_SPI_DLYBCS_OFFSET);
}

void init_SPI_clk(volatile avr32_spi_t *spi, unsigned pba_hz, unsigned baudrate){
	// initialize the SPI clock
	uint8_t err_flag = 0;
	uint8_t div = 0;
	// Configure the baudrate
	// SPI depends on PBA clock
	uint8_t pb = (uint8_t)(pba_hz/1000000);
	uint8_t br = (uint8_t)(baudrate/1000000);
	div = (uint8_t)(pb/br);
	
	if(div<=0 || div>255){
		err_flag = 1;
		return;
	}
	
	// Although we are only using 1 chip (ergo fixed CS),
	// clock generation is set in the CSRn registers.
	// Using CSR0
	spi->csr0 = (0 << AVR32_SPI_DLYBCT_OFFSET | 0 << AVR32_SPI_DLYBS_OFFSET |
					div << AVR32_SPI_SCBR_OFFSET | AVR32_SPI_BITS_16_BPT << AVR32_SPI_BITS_OFFSET |
					0 << AVR32_SPI_CSAAT_OFFSET | 0 << AVR32_SPI_NCPHA_OFFSET | 
					1 << AVR32_SPI_CPOL_OFFSET | 1 << AVR32_SPI_CSNAAT_OFFSET);
}

void write_spi(volatile avr32_spi_t *spi, uint16_t data){
	spi->tdr = (data << 2) << AVR32_SPI_TDR_TD_OFFSET | 14 << AVR32_SPI_TDR_PCS_OFFSET;
}

void enable_spi(volatile avr32_spi_t *spi){
	spi->cr = (1 << AVR32_SPI_SPIEN_OFFSET);
}

void disable_spi(volatile avr32_spi_t *spi){
	spi->cr = (1 << AVR32_SPI_SPIDIS_OFFSET);
}