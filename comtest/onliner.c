#ifndef F_CPU
#define F_CPU 9830400
#endif

#include "io.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>


#define UBRRVAL 25

void usart_init(void);
unsigned char urxchar(void);


int main(){ 
	
	DDRC |= (1<<PC0);
	
	DDRD |= (1 << PC1);
    PORTD &= ~(1 << PC1);

    usart_init();
   
   while (1){
	  
	unsigned char r = urxchar();

	if(r == 'g'){
		PORTC |= (1<<PC0);
	}
	else if (r=='n'){
        PORTC &= ~(1<<PC0);
	}
	else{

	}
	
  }

   return 0;
 }
 
void usart_init(void){
    //Set baud rate
	UBRRH= (unsigned char)(UBRRVAL>>8);   //high byte
    UBRRL=(unsigned char)UBRRVAL;     //low byte
    //Enable Transmitter and Receiver and Interrupt on receive complete
    UCSRB |= (1<<RXEN);
    //Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
    UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); 
}

unsigned char urxchar(void){
	while(!(UCSRA & (1<<RXC)));
		return UDR;
}