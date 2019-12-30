/*
 * Ex009.c
 *
 * Created: 12/30/2019 1:37:15 PM
 * Author : LS
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int int0Count=0; //counting the number of times int0 has occurred
void RED_LED_ON(void){ PORTB |= 0B00100000;} //PORTB bit 5 to 1
void RED_LED_OFF(void){ PORTB |= 0B11011111;} //PORTB bit 5 to 0
void GREEN_LED_ON(void){ PORTB |= 0B00000001;} //PORTB bit 5 to 1
void GREEN_LED_OFF(void){ PORTB |= 0B11111110;} //PORTB bit 5 to 1
	
void init_timer0_overflow_interrupt(void){
	TCNT0=0x00;
	TIMSK0 |= 0x01;
	TCCR0A &= 0xFC;
	TCCR0B &= 0xF7;
	TCCR0B |= 0x04;
	TCCR0B |= 0xFC;
	
}
	


int main(void)
{
	
    DDRB |= 0b00100001;
	GREEN_LED_ON();
	RED_LED_OFF();
	init_timer0_overflow_interrupt();
	sei();
    while (1) 
    {
		if(int0Count >= 1000){
		}
		int0Count=0;
		RED_LED_ON();
    }
}

ISR(TIMER0_OVF_vect){
	int0Count++;
}

