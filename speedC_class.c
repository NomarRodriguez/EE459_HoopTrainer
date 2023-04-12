#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 


void speedC_init(void){
	// motor 2
	DDRD |= 1 << DDD5;
	DDRD |= 1 << DDD6;
	
	// motor 1
	DDRD |= 1 << DDD7;
	DDRB |= 1 << DDB0;
	
}

void speedC_1_set(int input){
	
	if (input == 1){
		// forward 
		PORTD |= 1 << PD7; 
		PORTB &= ~(1 << PB0);
	}else if (input == -1){
		// reverse 
		PORTB |= 1 << PB0; 
		PORTD &= ~(1 << PD7);
	}else if (input == 0){
		// stop
		PORTD &= ~(1 << PD7);
		PORTB &= ~(1 << PB0);
	}
}


void speedC_2_set(int input){
	
	if (input == 1){
		// forward 
		PORTD |= 1 << PD5; 
		PORTD &= ~(1 << PD6);
	}else if (input == -1){
		// reverse 
		PORTD |= 1 << PD5; 
		PORTD &= ~(1 << PD6);
	}else if (input == 0){
		// stop
		PORTD &= ~(1 << PB5);
		PORTD &= ~(1 << PB6);
	}
}