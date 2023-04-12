#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "adc.h"

// only neccesary if running irbeam_test
void irbeam_init(void){

	DDRC |= 1 << DDC0;
	
}

void irbeam_test(void){
	if ((PIND & (1 << 3)) == 0){
		PORTC |= 1 << PC0; 
	}else{
		PORTC &= ~(1 << PC0);
	}
}

int irbeam_state(void){
	if ((PIND & (1 << 3)) == 0){
		return 1;
	}else{
		return 0;
	}
}