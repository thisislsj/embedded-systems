/*
 * Ex006.c
 *
 * Created: 12/16/2019 12:59:23 PM
 * Author : LS
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB |= 0B00100000; /*PortB pin5 as output*/
	DDRD &= 0B01111111; /*PortD pin7 as input*/
	DDRD &= 0B10111111; /*PortD pin6 as input*/
	
    while (1) 
    {/* Check if port D pin is LOW */
		if ((PIND & 0B01000000) &&((PIND & 0B10000000)==0))
		{	/* Make portB pin 5 HIGH */
			PORTB |= 0B00100000; //only the 5th bit is set to One
		}
		else
		{	/* Make portB pin 5 LOW */
			PORTB &= 0B11011111; //only the 5th bit is set to zero
		}
		_delay_ms(500); /*Wait for half a second*/ //delay function holds the processor from doing anything
    }
}

