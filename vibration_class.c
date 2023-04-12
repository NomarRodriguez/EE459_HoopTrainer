#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "adc.h"

void vibration_init(void){
	// Initialize the ADC
    adc_init();
	

	DDRC |= 1 << DDC0;
	
}

void vibration_test(void){
	char adc_resultx = 0; 
	adc_resultx = adc_sample(2);
	
	if (adc_resultx >= 40){
		PORTC |= 1 << PC0; 
	}else{
		PORTC &= ~(1 << PC0);
	}
}

int vibration_state(void){
	char adc_resultx = 0; 
	adc_resultx = adc_sample(2);
	
	if (adc_resultx >= 500){
		return 1;
	}else{
		return 0;
	}
}