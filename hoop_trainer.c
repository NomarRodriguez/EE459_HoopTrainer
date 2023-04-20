#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#include "vibration_class.h"
#include "servo_class.h"
#include "fsr_class.h"
#include "irbeam_class.h"
#include "speedC_class.h"

#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>	


// 7372800 or 9830400  for FOSC
#define FOSC 7372800           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register

#define RCVD_BUF_SIZE  6        // Maximum size of speed data, including @ and '\0'

#define SERIAL_START '['
#define SERIAL_END   ']'


static volatile unsigned char rcount, recv_full, recv_start;
static volatile char rbuf[RCVD_BUF_SIZE];


//global variables for interrupt
volatile char buf[18];
volatile int idx = 0;
volatile int rec_flag = 0;
volatile int fullMess = 0;
volatile char ch = 0;

void shot_attempt_counter(void);
void shot_make_counter(void);
void drill_set(int drill_number);

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

int shot_attempts = 0;
int shot_makes = 0;

int drill_flag = 0;


int main(void)
{
	servo_init();
	vibration_init();
	fsr_init();
	irbeam_init();
	DDRC |= 1 << DDC0;
	DDRC |= 1 << DDC1;
	DDRC |= 1 << DDC2;
	//int test_variable;
	speedC_init();
	
	serial_init(MYUBRR);
	int message_r_flag = 0;
	
	
    while (1) {
		speedC_1_set(1);
		speedC_2_set(1);
		serial_stringout("Drill 1");
		message_r_flag = recieved_message("Drill 1");
		
		if (message_r_flag){
			drill_set(1);
		}else{
			drill_set(2);
		}
		
		shot_attempt_counter();
		shot_make_counter();
		
		if (shot_attempts == 5) {
			PORTC |= 1 << PC1;
			//shot_attempts = 0;
		}else{
			PORTC &= ~(1 << PC1);
		}
		
		if (shot_attempts >= 7){
			shot_attempts = 0;
		}
		
		if (shot_makes == 5) {
			PORTC |= 1 << PC2;
			//shot_makes = 0;
		}else{
			PORTC &= ~(1 << PC2);
		}
		
		if (shot_makes >= 7){
			shot_makes = 0;
		}
		//speedC_1_set(1);

		
		
		//drill_set(1);
		
		//set_servo(185);
		
		
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

// not ideal because delay is used but it works 
void shot_attempt_counter(void){
		int ir_state = 0;
		int ir_flag = 0;
		int vibe_state = 0;
		int vibe_flag = 0;
		ir_state = irbeam_state();
		vibe_state = vibration_state();
		
		if (((ir_state == 1) && (ir_flag == 0)) || ((vibe_state == 1) && (vibe_flag == 0))){
			shot_attempts = shot_attempts + 1;
			_delay_ms(500);
			ir_flag = 1;
			vibe_flag = 1;
		}else{
			ir_flag = 0;
			vibe_flag = 0;
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

void drill_set(int drill_number){
	if(drill_flag == 0){
		// add more drills as needed 
		if (drill_number == 1){
			set_servo(185);
			//wait 5 seconds
			_delay_ms(5000);
			
			set_servo(100);
			
			//speedC_1_set(1);
			//speedC_2_set(1);
		

			
			
			// turn on return motors 
			_delay_ms(5000);
			set_servo(50);
			//speedC_1_set(1);
			//speedC_2_set(1);


			
			//turn on return motors
			// when user gives input reset drill flag back to 0 
			drill_flag = 1;
		}else if(drill_number == 2){
			set_servo(100);
		}
	}
}


void serial_init(unsigned short ubrr_value)
{

    // Set up USART0 registers
	// pc4 for hoop side or pc1 for lcd side
    // Enable tri-state buffer
    //DDRC |= (1 << PC1);
    //PORTC &= ~(1 << PC1);
	UBRR0 = MYUBRR; //set buad rate
    UCSR0C = (3 << UCSZ00);               // Async., no parity,
                                          // 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
	DDRC |= (1 << PC4);
    PORTC &= ~(1 << PC4);
	
	UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    sei();                      // Enable interrupts


}

void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = ch;
}

void serial_stringout(char *s)
{

    // Call serial_txchar in loop to send a string
    // (Very similar to lcd_stringout in lcd.c)
    int i = 0;
    while (s[i] != '\0') {    // Loop until next charater is NULL byte
        serial_txchar(s[i]);  // Send the character
        i++;
    }

}

int recieved_message(char message[]){
	char *ret;
	ret = strstr( buf, message);
		// 0 turns on led in lcd side
	if (ret){
		PORTC |= 1 << PC0; 
		//PORTC &= ~(1 << PC0);
		return 1;
	}else{
		PORTC &= ~(1 << PC0);
		return 0;
		//PORTC |= 1 << PC0;
	}	
}



ISR(USART_RX_vect)
{

    // Handle received character
    char ch;

    ch = UDR0;

    buf[idx++] = ch;
    if(idx == 16){
        buf[++idx] = '\0';
        idx = 0;
    }
}
