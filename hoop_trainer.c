#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "vibration_class.h"
//#include "servo_class.h"
#include "fsr_class.h"
#include "irbeam_class.h"

int main(void)
{
	//servo_init();
	//vibration_init();
	fsr_init();
	irbeam_init();
	
    while (1) {
		//test_servo();
		//set_servo(346);
		//vibration_test();
		//irbeam_test();
		fsr_test();
	}

    return 0;   /* never reached */
}