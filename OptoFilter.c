/*
 * OptoFilter.c
 *
 * Created: 11/11/2013 5:58:27 PM
 *  Author: Caroline
 */ 

#include "OptoFilter.h"

uint16_t fir_bpf(uint16_t *signal_data, uint8_t string_num){
	
	// TODO: Finish implementing bandpass filtering
	// NOTE: This current filtering code was commented out 
	// in currently running release of project as per request
	// of customer
	
	// filter response information
	dsp16_t coeff_size = 3; // same for all strings; 2 taps
	dsp16_t input_size = 3;
	dsp16_t str1_coeff[3] = {1,-1.77885,0.783044};
	dsp16_t str2_coeff[3] = {1,-1.81624,0.819037};
	dsp16_t str3_coeff[3] = {1,-1.82759,0.829479};
	dsp16_t str4_coeff[3] = {1,-1.8293,0.830861};
	dsp16_t str5_coeff[3] = {1,-1.83894,0.840029};
	dsp16_t str6_coeff[3] = {1,-1.88072,0.881328};
	
	dsp16_t str1_buf[3] = {signal_data[0],signal_data[0],signal_data[0]};
	dsp16_t str2_buf[3] = {signal_data[1],signal_data[1],signal_data[1]};
	dsp16_t str3_buf[3] = {signal_data[2],signal_data[2],signal_data[2]};
	dsp16_t str4_buf[3] = {signal_data[3],signal_data[3],signal_data[3]};
	dsp16_t str5_buf[3] = {signal_data[4],signal_data[4],signal_data[4]};
	dsp16_t str6_buf[3] = {signal_data[5],signal_data[5],signal_data[5]};
	
	// filtered signal
	dsp16_t filt_sig[1] = {0};
	
	// Bandpass Filter
	if(string_num < 1 || string_num > 6){
		// Invalid string number
		return 0;
	}
	switch(string_num){
		case 1:
			dsp16_filt_fir(filt_sig, str1_buf, input_size, str1_coeff, coeff_size);
			break;
		case 2:	
			dsp16_filt_fir(filt_sig, str2_buf, input_size, str2_coeff, coeff_size);
			break;
		case 3:
			dsp16_filt_fir(filt_sig, str3_buf, input_size, str3_coeff, coeff_size);
			break;
		case 4:
			dsp16_filt_fir(filt_sig, str4_buf, input_size, str4_coeff, coeff_size);
			break;
		case 5:
			dsp16_filt_fir(filt_sig, str5_buf, input_size, str5_coeff, coeff_size);
			break;
		case 6:
			dsp16_filt_fir(filt_sig, str6_buf, input_size, str6_coeff, coeff_size);
			break;
		default:
			return 0;
	}
	filt_sig = (uint16_t)filt_sig;
	return filt_sig;
}

uint16_t sum_signal(uint16_t *signal_data){
	// summation
	
	// TODO: Fix summation code to account for sign
	// Make hardware adjustments and test as described
	// on EDGE before changing any code
	
	int i = 0;

	uint16_t bias = 0x01FF; // Bias set to half way for 10 bits
	uint16_t signal_sum;
	int16_t signed_sum;
	
	signal_sum = 0;
	
	for(i = 0; i<6; i++){
		if(signed_sum == 0x3FF){ // 10 bit maximum value
			return 0x3FF;
		}
		signed_sum += (signal_data[i] - bias);
	}
	signal_sum = (uint16_t)signed_sum + bias;
	return signal_sum;
}