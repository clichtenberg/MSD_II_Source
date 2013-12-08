/*
 * OptoGuitar.c
 *
 * 
 *  Author: Caroline Lichtenberger
 * 
 *	NOTE 1: unless preceded by Opto in the header file name,
 *	header file is property of and authored by Atmel Corporation
 *	(see copyright notices in respective header/source files)
 */ 

#include <avr32/io.h>
#include <stdint.h>
#include "compiler.h"
#include "intc.h"
#include "interrupt/interrupt_avr32.h"
#include "dsp.h"

#include "avr32\uc3l064.h"
#include "avr32\adcifb_101.h"
#include "avr32\pdca_122.h"
#include "avr32\pm_411.h"
#include "avr32\scif_102.h"
#include "avr32\tc_2231.h"
#include "avr32\spi_211.h"

#include "OptoDMA.h"
#include "OptoADC.h"
#include "OptoSPI.h"
#include "OptoGPIOConfig.h"
#include "OptoClk.h"
#include "OptoTC.h"
#include "OptoFilter.h"

#define cpu_freq	30000000
#define pb_freq		30000000
#define rcmhz_freq	120000000
#define spi_freq	5000000


/************************************************************************/
/* Globals needed in both main() and ISR                                */
/************************************************************************/
// ADC interface register map
volatile avr32_adcifb_t *adc = &AVR32_ADCIFB;
// DMA storage location for ADC channels
uint16_t adc_dma_loc[6];
uint16_t summed_sig = 0;
// DMA interface register map
avr32_pdca_channel_t *dma_adc;
// SPI interface for DAC communication
volatile avr32_spi_t *spi = &AVR32_SPI;
uint32_t twim_addr = 0x60; // device address
// TC for timing interrupt
volatile avr32_tc_channel_t *tc = &AVR32_TC0.channel[1];

/************************************************************************/
/* ISR                                                                  */
/************************************************************************/
__attribute__((__interrupt__))
static void opto_isr(void){
	uint32_t temp = 0;
	Disable_global_interrupt();

	summed_sig = 0;
	
	// Start ADC conversion
	adc_start(adc);
	
	summed_sig = sum_signal(adc_dma_loc);
	write_spi(spi, summed_sig);
	
	tc->idr = 0xFFFF;
	temp = tc->idr;
	initTC_interrupts(1, tc);
}


int main(void)
{
	// Need to ensure no interrupts trigger while configuring system
	Disable_global_interrupt();
	
	// Configure clock source to run at 30 MHz with RC120MHz clock
	// Default clock is RCSYS at 115 kHz
	// Note: these variables are for debugging purposes (i.e. ensure you
	// have correct clock settings)
	volatile avr32_pm_t *pm_set = &AVR32_PM;
	volatile avr32_scif_t *clk = &AVR32_SCIF;
	
	initClock(clk, pm_set);
	
	// initialize storage area
	for(int i = 0; i < 6; ++i){
		adc_dma_loc[i] = 0;
	}
	
	// init and enable DMA
	init_adc_dma(AVR32_PDCA_PID_ADC_RX, dma_adc, adc_dma_loc);
	
	// Configure and enable GPIO for ADC and SPI
	assign_gpio_module(AVR32_ADCIFB_AD_0_PIN, AVR32_ADCIFB_AD_0_FUNCTION);
	assign_gpio_module(AVR32_ADCIFB_AD_1_PIN, AVR32_ADCIFB_AD_1_FUNCTION);
	assign_gpio_module(AVR32_ADCIFB_AD_2_PIN, AVR32_ADCIFB_AD_2_FUNCTION);
	assign_gpio_module(AVR32_ADCIFB_AD_4_PIN, AVR32_ADCIFB_AD_4_FUNCTION);
	assign_gpio_module(AVR32_ADCIFB_AD_5_PIN, AVR32_ADCIFB_AD_5_FUNCTION);
	assign_gpio_module(AVR32_ADCIFB_AD_6_PIN, AVR32_ADCIFB_AD_6_FUNCTION);
	
	assign_gpio_module(AVR32_SPI_SCK_0_0_PIN, AVR32_SPI_SCK_0_0_FUNCTION);
	assign_gpio_module(AVR32_SPI_MOSI_0_0_PIN, AVR32_SPI_MOSI_0_0_FUNCTION);
	assign_gpio_module(AVR32_SPI_MISO_0_0_PIN, AVR32_SPI_MISO_0_0_FUNCTION);
	assign_gpio_module(AVR32_SPI_NPCS_0_0_PIN, AVR32_SPI_NPCS_0_0_FUNCTION);
	
	// Initialize ADC
	adc_init(adc);
	
	// Enable DMA for ADC
	enable_dma(AVR32_PDCA_PID_ADC_RX, dma_adc);
	
	// Enable ADC channels
	adc_enable(adc, AVR32_ADCIFB_CH0);
	adc_enable(adc, AVR32_ADCIFB_CH1);
	adc_enable(adc, AVR32_ADCIFB_CH2);
	adc_enable(adc, AVR32_ADCIFB_CH4);
	adc_enable(adc, AVR32_ADCIFB_CH5);
	adc_enable(adc, AVR32_ADCIFB_CH6);
	
	// Initialize SPI
	initSPI(spi);
	init_SPI_clk(spi, pb_freq, spi_freq);
	enable_spi(spi);
	
	// Initialize interrupt vectors
	INTC_init_interrupts();
	INTC_register_interrupt(&opto_isr, AVR32_TC0_IRQ1, AVR32_INTC_INT3);
	
	// Initialize TC
	initTC(1, tc);
	set_TC_RC(1, tc);
	initTC_interrupts(1, tc);
	start_TC(1, tc);

	// Wait for ISR (triggers every 0.021 ms or 48 kHz)
	while(1){
		// Wait for it....
	}
}