#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "vibration_class.h"
#include "servo_class.h"
#include "fsr_class.h"
#include "irbeam_class.h"
#include "speedC_class.h"

void shot_attempt_counter(void);
void shot_make_counter(void);

int shot_attempts = 0;
int shot_makes = 0;


int main(void)
{
	//servo_init();
	//vibration_init();
	fsr_init();
	irbeam_init();
	DDRC |= 1 << DDC0;
	//int test_variable;
	//speedC_init();
	
    while (1) {
		shot_attempt_counter();
		shot_make_counter();
		

		
		//speedC_1_set(0);
		//test_servo();
		//set_servo(346);
		//irbeam_test();
		//fsr_test();
		//vibration_test();
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

void shot_attempt_counter(void){
		int ir_state = 0;
		int ir_flag = 0;
		ir_state = irbeam_state();
		
		if ((ir_state == 1) && (ir_flag == 0)){
			shot_attempts = shot_attempts + 1;
			_delay_ms(500);
			ir_flag = 1;
		}else{
			ir_flag = 0;
		}
}

// very sensitive, may sometimes be off by +- 1 depending on how the sensor is feeling( it gets in its feels sometimes) 
void shot_make_counter(void){
		int fsr_s = 0;
		int fsr_flag = 0;
		fsr_s = fsr_state();
		
		while ((fsr_s == 1) && (fsr_flag == 0)){
			fsr_flag = 1;
			shot_makes = shot_makes + 1;
		}
		
		if ((fsr_s == 0) && (fsr_flag == 1)){
			fsr_flag = 0;
		}
}