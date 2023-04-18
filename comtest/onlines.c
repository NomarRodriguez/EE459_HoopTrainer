#ifndef F_CPU
#define F_CPU 9830400
#endif

#include <util/delay.h>
#include "io.h"

#define UBRRVAL 51

void usart_init(void);
void sendbyte(unsigned char);
	

int main(){ 
	
	
    usart_init();
   
   while (1){
	  
	UCSR0B |= (1<<TXEN0);
	sendbyte('g');
	UCSR0B &= ~(1<<TXEN0);

      }
   
   return 0;
 }
 
void usart_init(void){
    //Set baud rate
	UBRR0H= (unsigned char)(UBRRVAL>>8);   //high byte
    UBRR0L=(unsigned char)UBRRVAL;     			//low byte
	//UCSR0A |= (1<<U2X0);
	//Enable Transmitter and Receiver and Interrupt on receive complete
    UCSR0B |= (1<<TXEN0);
    //Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); 
}

void sendbyte(unsigned char MSG){
    // Wait if a byte is being transmitted
    while((UCSR0A&(1<<UDRE0)) == 0);
    // Transmit data
    UDR0 = MSG;
}