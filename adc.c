#include <avr/io.h>

#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC
    ADMUX = (ADMUX & 0b10000000);
    ADMUX = ADMUX | 0b01000000;
    ADMUX = ADMUX & 0b11110011;
    ADMUX = ADMUX | 0b00000011;
    
    ADCSRA = ADCSRA | 0b00000111;
    
    ADMUX = ADMUX | 0b00100000;
    
    ADCSRA = ADCSRA | 0b10000000;
}

unsigned char adc_sample(unsigned char channel)
{
    // Set ADC input mux bits to 'channel' value
    unsigned char maskbits = channel & (0b00001111);
    
    ADMUX &= (0b11110000);
    ADMUX = ADMUX | maskbits;
    
    ADCSRA = ADCSRA | 0b01000000;
    while((ADCSRA & 0b01000000) != 0 )
    {}
    unsigned char result = ADCH;
    return result;
    // Convert an analog input and return the 8-bit result

}
