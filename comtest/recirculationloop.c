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

    UBRR0 = MYUBRR; //set buad rate
    UCSR0C = (3 << UCSZ00);               // Async., no parity,
                                          // 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
    serial_init(MYUBRR);

    
	//check9_init();

    // Enable interrupts
    UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    //sei();                      // Enable interrupts

	char sent_message[] = "[AAAA]\n";
	char rec_message[] = "[AAAA]\n";


    while (1) {                 // Loop forever
		serial_stringout(sent_message);
		// 0 turns on led in lcd side
		if (buf == "AAAA\n"){
			//PORTC |= 1 << PC0; 
			PORTC &= ~(1 << PC0);
		}else{
			//PORTC &= ~(1 << PC0);
			PORTC |= 1 << PC0;
		}

        //for with polling instead of interrupts
		ch = UDR0;
		if (ch == SERIAL_START) {   // First character of string?
	        recv_start = 1;		// Flag that start character received
	        rcount = 0;		// Index for where next character goes in rbuf
	        recv_full = 0;          // Clear flag for rbuf full
            finish_recv();
        }
		
		
    }
}

/* ----------------------------------------------------------------------- */
//polling function
void finish_recv(){
    while(!recv_full || recv_start){
        if (ch == SERIAL_END) { // End of transmission?
	        if (rcount > 1) {	// Anything received?/
		        rbuf[rcount] = '\0'; // Terminate the string
		        recv_full = 1;  // Set flag for data received
	        }
	    recv_start = 0;	// Packet complete
        }
        else if ((ch >= '0' && ch <= '9') || ch == ' ') { // Check for 0-9 and space
	        if (rcount < RCVD_BUF_SIZE-1){		  // Leave room for the '\0'
		        rbuf[rcount++] = ch;  // Put in buffer
            }
	        else{
		        recv_start = 0;	// Too much data, reset the receiver
            }
        }
	    else{
	        recv_start = 0;	// Bad data so reset the receiver
        }
    }
}

void serial_init(unsigned short ubrr_value)
{

    // Set up USART0 registers
	// pc4 for hoop side or pc1 for lcd side
    // Enable tri-state buffer
    DDRD |= (1 << PC1);
    PORTD &= ~(1 << PC1);
	rcount = 0;                 // Count of characters received
    recv_full = 0;              // Flag for received buffer full
    recv_start = 0;		// Flag for start character received
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

unsigned char recv_string(char *rp)
{
    unsigned char status;
    char ch, *p;

    cli();
    if (recv_full) {		// See if new data in rbuf
	// Could use strcpy(rp, rbuf);
	p = (char *) rbuf;
	while ((ch = *p++) != '\0')
	    *rp++ = ch;
	*rp = '\0';
	status = 1;             // Return status = 1
	recv_full = 0;	        // Clear flag for rbuf full
    }
    else
	status = 0;             // If nothing, return 0
    //sei();
    return(status);
}

ISR(USART_RX_vect)
{

    // Handle received character
    char ch;

    ch = UDR0;

    buf[idx++] = ch;
    if(idx == 16){
		rec_flag = 1;
        buf[++idx] = '\0';
        idx = 0;
    }
}
