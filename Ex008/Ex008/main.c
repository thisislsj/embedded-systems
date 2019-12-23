/*
 * Ex008.c
 *
 * Created: 12/23/2019 2:56:56 PM
 * Author : LS
 */ 
#define F_CPU 16000000
#define BAUD 9600
#define MYUBRR ((F_CPU/16/BAUD)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_Init(unsigned int);
void USART_Transmit(unsigned char);
unsigned char USART_Receive(void);
void EnableADC(void);
unsigned int read_adc(unsigned char);
void LED_ON(void){
	PORTB |= 0B00100000;
}

void LED_OFF(void){
	PORTB &= 0B11011111;
}

void LED_PORT_CONFIG(void){
	DDRB |= 0B00100000;
}




unsigned char V0=0, V1=0, V2=0, V3=0;

volatile unsigned int valueADC;
volatile unsigned int flagADC;



int main(void)
{
    LED_PORT_CONFIG();
	LED_OFF();
	USART_Init(MYUBRR);
	EnableADC();
    while (1) 
    {
		if (flagADC==1){
			flagADC=0;
			LED_OFF();
			_delay_ms(500);
			
			
			
				USART_Transmit('>');
				V0=valueADC%10;
				V1=(valueADC/10)%10;
				V2=(valueADC/100)%10;
				V3=(valueADC/1000)%10;
				USART_Transmit(0x30+V3);
				USART_Transmit(0x30+V2);
				USART_Transmit(0x30+V1);
				USART_Transmit(0x30+V0);
				USART_Transmit(0x0A); USART_Transmit(0x0D);
				
				LED_ON();
				_delay_ms(500);
				ADCSRA |= (1<<ADSC);
				
				
			
			
			
		}
		
    }
	return 0;
	

}

void EnableADC(void){
	ADCSRA |= (1<<ADEN);
	ADCSRA |= 0x07;
	
	ADMUX = (ADMUX & 0xF0) | 0x00;
	ADCSRA |= (1<<ADIE);
	sei();
	ADCSRA |= (1<<ADSC);
}

unsigned int read_adc(unsigned char adc_input_pin){
	ADMUX = (ADMUX & 0xF0) | adc_input_pin;
	ADMUX &= 0x3F;
	
	ADCSRA |= (1<< ADSC);
	
	while (ADCSRA & (1<<ADSC));
	
	return ADC;
}

void USART_Init(unsigned int ubrr){
	UBRR0H= (unsigned char)(ubrr>>8);
	UBRR0L=(unsigned char)ubrr;
	UCSR0B=(1<< RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C=(3<<UCSZ00);
}

void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=data;
}

ISR(ADC_vect){
	valueADC=ADC;
	flagADC=1;
}



