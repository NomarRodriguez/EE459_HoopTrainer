#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>

#include "serial.h"

void serial_txchar(char);

#define RCVD_BUF_SIZE  6        // Maximum size of speed data, including @ and '\0'

static volatile unsigned char rcount, recv_full, recv_start;
static volatile char rbuf[RCVD_BUF_SIZE];

//global variables for interrupt
volatile char buf[18];
volatile int idx = 0;
volatile int fullMess = 0;

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short ubrr_value)
{

    // Set up USART0 registers

    // Enable tri-state buffer
    DDRD |= (1 << PD3);
    PORTD &= ~(1 << PD3);

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
        fullMess = 1;
        buf[++idx] = '\0';
        idx = 0;
    }
}