/*
 * Ex002.c
 *
 * Created: 11/25/2019 2:36:23 PM
 * Author : LS
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	DDRB |= 0B00100000;
    /* Replace with your application code */
    while (1) 
    {
		PORTB |= 0B00100000;
		_delay_ms(300);
		PORTB &= 0B11011111;
		_delay_ms(300);
    }
}

