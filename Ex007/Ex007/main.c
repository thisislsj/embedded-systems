/*
 * Ex007.c
 *
 * Created: 12/23/2019 1:11:20 PM
 * Author : LS
 */ 
#define F_CPU 16000000 //Clock Speed
#define BAUD 9600
#define MYBURR ((F_CPU/16/BAUD)-1)

#include <util/delay.h>
#include <avr/io.h>

void USART_Init(unsigned int ubrr){
	//Set baud rate
	UBRR0H=(unsigned char)(ubrr>>8);
	UBRR0L=(unsigned char)ubrr;
	//Enable receiver and transmitter
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	//Set frame format: 8data
	UCSR0C=(3<<UCSZ00);
}

void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=data;
}

void USART_Receive(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}


void LED_ON(void){
	PORTB |= 0B00100000;
}

void LED_OFF(void){
	PORTB &= 0B11011111;
}

void LED_PORT_CONFIG(void){
	DDRB |= 0B00100000;
}

int main(void)
{
    LED_PORT_CONFIG();
	LED_OFF();
	LED_ON();
    while (1) 
    {
		LED_ON();
		USART_Transmit('H');
		USART_Transmit('E');
		USART_Transmit('L');
		USART_Transmit('L');
		USART_Transmit('O');
		USART_Transmit(0x0A); USART_Transmit(0x0D);
		_delay_ms(250);
		LED_OFF();
		_delay_ms(250);
    }
	return 0;
}

