/*
 * OptoGuitar.c
 *
 * 
 *  Author: Caroline Lichtenberger
 */ 

#include "avr32\uc3l064.h"
#include "avr32\adcifb_101.h"
#include "avr32\pdca_122.h"
#include "avr32\pm_411.h"
#include "avr32\scif_102.h"
#include "avr32\twim_101.h"

#include "OptoDMA.h"
#include "OptoADC.h"
#include "OptoTWIM.h"
#include "OptoGPIOConfig.h"
#include "OptoClk.h"

#include <avr32/io.h>
#include <stdio.h>
#include <stdint.h>

#define cpu_freq	30000000
#define pb_freq		30000000
#define rcmhz_freq	120000000

int main(void)
{
	// Configure clock source to run at 30 MHz with RC120MHz clock
	// Default clock is RCSYS at 115 kHz
	volatile avr32_pm_t *pm_set = &AVR32_PM;
	volatile avr32_scif_t *clk = &AVR32_SCIF;
	
	initClock(clk, pm_set);
			
	// ADC interface register map
	volatile avr32_adcifb_t *adc = &AVR32_ADCIFB;
	
	// DMA storage location for ADC channels
	uint16_t adc_dma_loc[6];		// make array[6]
	uint16_t y = 0;
	uint16_t summed_sig = 0;
	uint8_t twim_buffer;
	// initialize storage area
	for(int i = 0; i < 6; ++i){
		adc_dma_loc[i] = 0;
	}
	
	// DMA interface register map
	avr32_pdca_channel_t *dma_adc;
	avr32_pdca_channel_t *dma_twim;
	
	// TWIM interface for DAC communication
	volatile avr32_twim_t *twi = &AVR32_TWIM1;
	uint32_t twim_addr = 0x60; // device address
	
	// TC for timing interrupt
	avr32_tc_t *tc;
	
	// init and enable DMA
	init_adc_dma(AVR32_PDCA_PID_ADC_RX, dma_adc, adc_dma_loc);
	
	/************************************************************************/
	/* GPIO Initialization                                                   */
	/************************************************************************/
	// Enable ADC and TWIM functionalities
	
	enable_gpio_pin(AVR32_PIN_PA17);
	enable_gpio_pin(AVR32_PIN_PA05);
	
	assign_gpio_module(AVR32_ADCIFB_AD_0_PIN, AVR32_ADCIFB_AD_0_FUNCTION);
	
	/**assign_gpio_module(AVR32_TWIMS0_TWCK_0_0_PIN, AVR32_TWIMS0_TWCK_0_0_FUNCTION);
	assign_gpio_module(AVR32_TWIMS0_TWD_0_0_PIN, AVR32_TWIMS0_TWD_0_0_FUNCTION);
	assign_gpio_module(AVR32_TWIMS0_TWD_0_1_PIN, AVR32_TWIMS0_TWD_0_1_FUNCTION);
	assign_gpio_module(AVR32_TWIMS0_TWCK_0_2_PIN, AVR32_TWIMS0_TWCK_0_2_FUNCTION);
	assign_gpio_module(AVR32_TWIMS0_TWD_0_2_PIN, AVR32_TWIMS0_TWD_0_2_FUNCTION);
	assign_gpio_module(AVR32_TWIMS0_TWCK_0_3_PIN, AVR32_TWIMS0_TWCK_0_3_FUNCTION);*/
	
	assign_gpio_module(AVR32_TWIMS1_TWCK_0_0_PIN, AVR32_TWIMS1_TWCK_0_0_FUNCTION);
	assign_gpio_module(AVR32_TWIMS1_TWD_0_PIN, AVR32_TWIMS1_TWD_0_FUNCTION);
	
	//assign_gpio_module(AVR32_ADCIFB_AD_1_PIN, AVR32_ADCIFB_AD_1_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_2_PIN, AVR32_ADCIFB_AD_2_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_4_PIN, AVR32_ADCIFB_AD_4_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_5_PIN, AVR32_ADCIFB_AD_5_FUNCTION);
	//assign_gpio_module(AVR32_ADCIFB_AD_6_PIN, AVR32_ADCIFB_AD_6_FUNCTION);
	
	// Initialize ADC
	adc_init(adc);
	
	/************************************************************************/
	/* TC Initialization                                                    */
	/************************************************************************/
	
	// Enable DMA for ADC
	enable_dma(AVR32_PDCA_PID_ADC_RX, dma_adc);
	
	// Enable ADC channels
	adc_enable(adc, AVR32_ADCIFB_CH0);
	//adc_enable(adc, AVR32_ADCIFB_CH1);
	//adc_enable(adc, AVR32_ADCIFB_CH2);
	//adc_enable(adc, AVR32_ADCIFB_CH4);
	//adc_enable(adc, AVR32_ADCIFB_CH5);
	//adc_enable(adc, AVR32_ADCIFB_CH6);
	
	/************************************************************************/
	/* Main Start                                                           */
	/************************************************************************/
	init_twim(twi, 400000, twim_addr, pb_freq);
	delay(1000);
	
	while(1){
	
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
		
		//summed_sig = adc_dma_loc[0];
		summed_sig = 828;
		
		// TWIM communication
		dac_call_cmd(twi, twim_addr);
		dac_wakeup_cmd(twi, twim_addr);
		
		dac_call_cmd(twi, twim_addr);
		dac_reset_cmd(twi, twim_addr);
		
		// DAC single write command
		dac_write_cmd(twi, twim_addr);
		
		// Ensure bus is idle
		twim_buffer = (uint8_t)((summed_sig & 0x0F00) >> 8);
		//while(!(twi->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twi->sr & AVR32_TWIM_SR_TXRDY_MASK));
		twim_write(twi, twim_buffer, 1, twim_addr);
		// Ensure bus is idle, then transfer
		twim_buffer = (uint8_t)(summed_sig & 0x00FF);
		//while(!(twi->sr & AVR32_TWIM_SR_IDLE_MASK) && !(twi->sr & AVR32_TWIM_SR_TXRDY_MASK));
		twim_write(twi, twim_buffer, 1, twim_addr);
		// Update all outputs
		dac_call_cmd(twi, twim_addr);
		dac_software_update(twi, twim_addr);
		
		y++;
	}
	
	
	//while(1){
		// wait for ISR (0.021 ms or 48 kHz)
		
	//}
}

// ISR
	
	// DSP code
	
	// send to DAC
	
	// read ADC channels