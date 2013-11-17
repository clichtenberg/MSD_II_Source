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
#include "avr32\twim_101.h"
#include "avr32\tc_2231.h"

#include "OptoDMA.h"
#include "OptoADC.h"
#include "OptoTWIM.h"
#include "OptoGPIOConfig.h"
#include "OptoClk.h"
#include "OptoTC.h"

#define cpu_freq	30000000
#define pb_freq		15000000
#define rcmhz_freq	120000000


/************************************************************************/
/* Globals needed in both main() and ISR                                */
/************************************************************************/
// ADC interface register map
volatile avr32_adcifb_t *adc = &AVR32_ADCIFB;
// DMA storage location for ADC channels
uint16_t adc_dma_loc[6];
uint16_t y = 0;
uint16_t summed_sig = 0;
// DMA interface register map
avr32_pdca_channel_t *dma_adc;
// TWIM interface for DAC communication
volatile avr32_twim_t *twi = &AVR32_TWIM1;
uint32_t twim_addr = 0x60; // device address
// TC for timing interrupt
volatile avr32_tc_channel_t *tc;


/************************************************************************/
/* ISR                                                                  */
/************************************************************************/
__attribute__((__interrupt__))
static void opto_isr(void){
	
	y = 0;
	summed_sig = 0;
	
	// Wait until ADC ready
	while(conversion_status(adc) == 0){
		// wait
	}
	// Start ADC conversion
	adc_start(adc);
	// Wait for conversion to finish
	while(conversion_status(adc) == 0){
		// wait
	}
	
	summed_sig = adc_dma_loc[0];
	//summed_sig = 2730;
	
	// TWIM communication
	dac_write_cmd(twi, twim_addr, summed_sig);
	dac_software_update(twi, twim_addr);
	
	y++;
}


int main(void)
{
	//board_init();
	
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
	
	// Configure and enable GPIO for ADC and TWIM
	enable_gpio_pin(AVR32_PIN_PA17);
	enable_gpio_pin(AVR32_PIN_PA05);
	
	assign_gpio_module(AVR32_ADCIFB_AD_0_PIN, AVR32_ADCIFB_AD_0_FUNCTION);
	
	assign_gpio_module(AVR32_TWIMS1_TWCK_0_0_PIN, AVR32_TWIMS1_TWCK_0_0_FUNCTION);
	assign_gpio_module(AVR32_TWIMS1_TWD_0_PIN, AVR32_TWIMS1_TWD_0_FUNCTION);
	
	//assign_gpio_module(AVR32_ADCIFB_AD_1_PIN, AVR32_ADCIFB_AD_1_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_2_PIN, AVR32_ADCIFB_AD_2_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_4_PIN, AVR32_ADCIFB_AD_4_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_5_PIN, AVR32_ADCIFB_AD_5_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_6_PIN, AVR32_ADCIFB_AD_6_FUNCTION);
	
	// Initialize ADC
	adc_init(adc);
	
	// Enable DMA for ADC
	enable_dma(AVR32_PDCA_PID_ADC_RX, dma_adc);
	
	// Enable ADC channels
	adc_enable(adc, AVR32_ADCIFB_CH0);
	//adc_enable(adc, AVR32_ADCIFB_CH1);
	//adc_enable(adc, AVR32_ADCIFB_CH2);
	//adc_enable(adc, AVR32_ADCIFB_CH4);
	//adc_enable(adc, AVR32_ADCIFB_CH5);
	//adc_enable(adc, AVR32_ADCIFB_CH6);
	
	// Initialize TWIM
	init_twim(twi, 400000, twim_addr, pb_freq);
	delay(1000);
	
	// Initialize interrupt vectors

	INTC_init_interrupts();
	INTC_register_interrupt(&opto_isr, AVR32_TC1_IRQ0, AVR32_INTC_INT3);
	
	initTC(0, tc);
	
	set_TC_RC(0, tc);
	
	initTC_interrupts(0, tc);
	
	start_TC(0, tc);
	uint8_t x = 0;
	// Wait for ISR (triggers every 0.021 ms or 48 kHz)
	Enable_global_interrupt();
	while(1){
		// Wait for it....
	}
}