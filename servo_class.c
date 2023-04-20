#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void pwm_init(void);

/*
  Define PHASE_CORRECT to use Phase Correct PWM mode,
  otherwise it uses the Fast PWM mode.
*/ 
//#define PHASE_CORRECT

#ifdef PHASE_CORRECT
/*
  Phase correct PWM uses a dual slope count so the count value should be half
  the count needed for the delay
  Count = Delay * Fosc / (Prescale * 2)
*/
#define PWM_PERIOD 1536		// 20msec (with prescaler=64)
#define MIN_PW       58         // 0.75msec
#define MAX_PW      173         // 2.25msec
#define STEP_PW       1
#else  // Fast PWM
/*
  Fast PWM uses a single slope count
  Count = Delay * Fosc / Prescale
*/
#define PWM_PERIOD 3072         // 20msec (with prescaler=64)
#define MIN_PW      115         // 0.75msec
#define MAX_PW      346         // 2.25msec
#define STEP_PW       2
#endif
#define INITIAL_PW  MIN_PW

#define START_PW      115         // 0.75msec
#define END_PW      846         // 2.25msec


volatile int width;


void servo_init(void){


    DDRB |= (1 << PB2);	        // Output for OCR1B

    width = INITIAL_PW;
    pwm_init();                 // Initialize TIMER1 for PWM

    sei();

    _delay_ms(500);
}

void test_servo(void){
	int i;
	for (i = MIN_PW; i < MAX_PW; i += STEP_PW) {
	    width = i;
	    _delay_ms(10);
	}
	_delay_ms(100);
	for (i = MAX_PW; i > MIN_PW; i -= STEP_PW) {
	    width = i;
	    _delay_ms(10);
	}
	_delay_ms(100);
}

// 0 middle , 646 left(you facing it), 346 right(you facing it)
// values may need to be redone if servo is shifted
void set_servo(int i){
	width = i;
	_delay_ms(1000);
	/*int i;
	for (i = START_PW; i < END_PW; i -= STEP_PW) {
		if( i == START_PW){
			_delay_ms(5000);
		}else if(i == ((END_PW - START_PW)/2))
		{
			_delay_ms(5000);
		}else if(i = (END_PW-1)){
			_delay_ms(5000);
		}
	    width = i;
		
	    _delay_ms(10);
	}*/
}

/*
  pwm_init - Initialize Counter/Timer 1 for PWM, either fast PWM or
  phase correct, depending on PHASE_CORRECT being defined.
*/
void pwm_init(void)
{
    OCR1A = PWM_PERIOD;		// Period = 20msec
    OCR1B = INITIAL_PW;		// Initial PWM pulse width
    TCCR1A |= (0b10 << COM1B0); // Output on OC1B
#ifdef PHASE_CORRECT
    // Set the four WGM1 bits to 1011 for phase correct PWM, OCR1A = TOP
    TCCR1B |= (0b10 << WGM12);
    TCCR1A |= (0b11 << WGM10);
#else // Fast PWM
    // Set the four WGM1 bits to 1111 for fast PWM, OCR1A = TOP
    TCCR1B |= (0b11 << WGM12);
    TCCR1A |= (0b11 << WGM10);
#endif
    TIMSK1 |= (1 << OCIE1A);    // Enable interrupt on count = OCR1A;
    TCCR1B |= (0b011 << CS10);  // Set for divide by 64, start timer
}

/*
  The TIMER1_COMPA ISR is run when the timer reaches the TOP value
  which is during the middle of the time the pulse is low.
*/
ISR(TIMER1_COMPA_vect)
{
    OCR1B = width;		// Load new count value
}