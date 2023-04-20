#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>	

#include "serial.h"
//#include "check9.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

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



int main(void) {

    // Initialize the LCD, ADC and serial modules
	DDRC |= 1 << DDC0;
	DDRC |= 1 << DDC1;
	DDRC |= 1 << DDC2;
	//PORTC |= 1 << PC0;

    UBRR0 = MYUBRR; //set buad rate
    UCSR0C = (3 << UCSZ00);               // Async., no parity,
                                          // 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
    serial_init(MYUBRR);

    
	//check9_init();

    // Enable interrupts
    UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    sei();                      // Enable interrupts

	char sent_message[] = "A";
	char rec_message[] = "[AAAA]\n";
	char empty_buf [] = "";


    while (1) {                 // Loop forever
		serial_stringout(sent_message);
		recieved_message("A");
		

		//PORTC |= 1 << PC0; 
        //for with polling instead of interrupts
		/*ch = UDR0;
		if (ch == SERIAL_START) {   // First character of string?
	        recv_start = 1;		// Flag that start character received
	        rcount = 0;		// Index for where next character goes in rbuf
	        recv_full = 0;          // Clear flag for rbuf full
            finish_recv();
        }*/
		
		
    }
}


void serial_init(unsigned short ubrr_value)
{

    // Set up USART0 registers
	// pc4 for hoop side or pc1 for lcd side
    // Enable tri-state buffer
    //DDRC |= (1 << PC1);
    //PORTC &= ~(1 << PC1);
	DDRC |= (1 << PC4);
    PORTC &= ~(1 << PC4);


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
