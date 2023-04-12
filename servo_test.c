#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 

#include "timer1_hal.h"

int main(void)

{
	pwm_init();
	servo_set(0,180);
	
	int16_t i = 0;
	
    while(1){
		for(i = 0; i <= 180; i++){
			servo_set(i,180);
			_delay_ms(40);
		}
		for(i = 180; i <= 180; i--){
			servo_set(i,180);
			_delay_ms(40);
		}		
    }
	return 0;
}
