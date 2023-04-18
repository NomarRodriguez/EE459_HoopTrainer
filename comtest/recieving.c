/********************************************
 *
 *  Name: Bailey Cislowski
 *  Email: bcislows@usc.edu
 *  Section: Wednesday 2:00
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>	
//#include "check9.h"
// 7372800 or 9830400  for FOSC

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

#define FOSC 7372800           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register



//global variables for interrupt
volatile char buf[18];
volatile int idx = 0;
volatile int rec_flag = 0;
volatile int fullMess = 0;

int main(void) {

    // Initialize the LCD, ADC and serial modules
	DDRC |= 1 << DDC0;

    UBRR0 = MYUBRR; //set buad rate
    UCSR0C = (3 << UCSZ00);               // Async., no parity,
                                          // 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
    serial_init(MYUBRR);

    
	//check9_init();

    // Enable interrupts
    UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    sei();                      // Enable interrupts

	char message[] = "D";

    while (1) {                 // Loop forever
		char *ret;
		ret = strstr( buf, "D");
		if (ret){
			//PORTC |= 1 << PC0; 
			// for lcd side, 0 turns on led
			PORTC &= ~(1 << PC0);
		}else{
			//PORTC &= ~(1 << PC0);
			PORTC |= 1 << PC0;
		}
		
    }
}

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short ubrr_value)
{

    // Set up USART0 registers
	// pc4 for hoop side or pc1 for lcd side
    // Enable tri-state buffer
    DDRD |= (1 << PC1);
    PORTD &= ~(1 << PC1);
	//DDRD |= (1 << PC4);
    //PORTD &= ~(1 << PC4);


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

ISR(USART_RX_vect)
{

    // Handle received character
    char ch;

    ch = UDR0;

    buf[idx++] = ch;
    if(idx == 16){
		rec_flag == 1;
        buf[++idx] = '\0';
        idx = 0;
    }
}
