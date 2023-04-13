#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "adc.h"

void fsr_init(void){
	// Initialize the ADC
    adc_init();
	

	DDRC |= 1 << DDC0;
	
}

void fsr_test(void){
	char adc_resultx = 0; 
	adc_resultx = adc_sample(3);
	
	if (adc_resultx >= 10){
		PORTC |= 1 << PC0; 
	}else{
		PORTC &= ~(1 << PC0);
	}
}

int fsr_state(void){
	char adc_resultx = 0; 
	adc_resultx = adc_sample(3);
	
	if (adc_resultx >= 126){
		return 1;
	}else{
		return 0;
	}
}