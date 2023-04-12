#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "vibration_class.h"
#include "servo_class.h"
#include "fsr_class.h"
#include "irbeam_class.h"

int main(void)
{
	//servo_init();
	vibration_init();
	fsr_init();
	//irbeam_init();
	//DDRC |= 1 << DDC0;
	int test_variable;
	
    while (1) {
		//test_servo();
		//set_servo(346);
		//irbeam_test();
		//fsr_test();
		vibration_test();
		//set_servo(646);
		//_delay_ms(200);
		//PORTC |= 1 << PC0; 
		/*if (vibration_state == 1){
			test_variable = 1;
		}
		if (test_variable == 1){
			PORTC |= 1 << PC0;
		}else{
			PORTC &= ~(1 << PC0);
		}*/

	}

    return 0;   /* never reached */
}