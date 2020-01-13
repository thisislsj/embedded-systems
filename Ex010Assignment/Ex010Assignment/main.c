/*
 * LCD Display.c
 *
 * Created: 9/14/2018 11:32:53 PM
 * Author : Subodha Gunawardena
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define E_PIN			2
#define RS_PIN			3
#define LCD_CTRL_PORT_DIR	DDRB
#define LCD_CTRL_PORT_DATA	PORTB
#define LCD_DATA_PORT_DIR		DDRD
#define LCD_DATA_PORT_DATA		PORTD

void hardwareInit(void);				/* All hardware configurations should go here */
void hardwareInitLCD(void);				/* This goes into hardwareInit() function */

void sendCommand2LCD(unsigned char);/* Send a command to LCD */
void sendData2LCD(unsigned char);	/* Send a character to LCD */
void push2LED(void);				/* This function is used by send character/command to LCD functions*/
void setCursorLCD(char, char);		/* Set the cursor to (Row, Column) of the LCD */
void initLCD(void);					/* Initialize the LCD with predefined sequence */
void displayValue(unsigned int);	/* Display a value passed into it as 4 digit number */
void init_timer1_comp_interrupt(void);

unsigned int count = 0; /* define a count to be incremented */

int main(void)
{
	hardwareInit(); /* All hardware configurations should go here */
	
	initLCD();			/* Initialize the LCD with predefined sequence */
	setCursorLCD(1,1);	/* Set the cursor to Row1 Column1 of the LCD */
	sendData2LCD('S');	/* Display character S */
	sendData2LCD('T');  /* Display character T */
	sendData2LCD('A');  /* Display character A */
	sendData2LCD('R');  /* Display character R */
	sendData2LCD('T');  /* Display character T */
	
	init_timer1_comp_interrupt();
	sei();
	
    while (1) 
    {		//_delay_ms(100);		/* Wait for 1 second */
			//displayValue(count);	/* Display count as a 4 digit value */
			//count++;				/* Increment the count */
	}
}
/******************************************************/
/******************************************************/
void hardwareInit(void)
{
	hardwareInitLCD();
	/* Hardware configurations shuold come here */
}
/******************************************************/
/* Display a value as a four digit number at the LCD  */
/******************************************************/
void displayValue(unsigned int value)
{

		uint8_t V0, V1, V2, V3, V4, V5, V6;
		
		
		int minutes = value/(1000*60);
		int seconds = value/1000-minutes*60;
		int millis = value-seconds*1000-minutes*1000*60;
		
		V0 = millis % 10;		
		V1 = (millis/10)%10;		
		V2 = (millis/100)%10;	
		V3 = seconds % 10;
		V4 = (seconds/10)%10;
		V5 = minutes % 10;
		V6 = (minutes/10)%10;
		
		sendCommand2LCD(0x01);  //clear display
		setCursorLCD(1,7);		//Set cursor to 1st Row 7th Column
		sendData2LCD(0x30+V6);  //4th digit
		sendData2LCD(0x30+V5);  //3rd digit
		sendData2LCD(0x3A);  //3rd digit
		sendData2LCD(0x30+V4);  //2nd digit
		sendData2LCD(0x30+V3);  //1st digit
		sendData2LCD(0x3A);  //3rd digit
		sendData2LCD(0x30+V2);  //2nd digit
		sendData2LCD(0x30+V1);  //1st digit
		sendData2LCD(0x30+V0);  //1st digit
		
		
}
/****************************************************/
/*		Functions for 	LCD		Display				*/
/****************************************************/
void hardwareInitLCD(void)
{
	LCD_CTRL_PORT_DIR |= ((1<<E_PIN) | (1<<RS_PIN));	//LED control pins to output
	LCD_DATA_PORT_DIR = 0xFF;							//LED Data to output
}
/* Push values at port to LCD (used by sendCommand2LCD and sendData2LCD) */
void push2LED(void)
{
	LCD_CTRL_PORT_DATA |= (1<<E_PIN);
	//asm volatile ("nop");
	//asm volatile ("nop");
	_delay_ms(1);
	LCD_CTRL_PORT_DATA &= ~(1<<E_PIN);
	_delay_ms(1);
}
/* Send a command to LCD */
void sendCommand2LCD(unsigned char command)
{
	LCD_DATA_PORT_DATA = command;
	LCD_CTRL_PORT_DATA &= ~(1<<RS_PIN);
	push2LED();
	LCD_DATA_PORT_DATA = 0x00;
}
/* Send a character to LCD */
void sendData2LCD(unsigned char data)
{
	LCD_DATA_PORT_DATA = data;
	LCD_CTRL_PORT_DATA |= (1<<RS_PIN);
	push2LED();
	LCD_DATA_PORT_DATA = 0x00;
}
/* Set the cursor to Row, Column of the LCD */
void setCursorLCD(char a, char b)
{
	if(a == 1)
	sendCommand2LCD(0x80 + b);
	else if(a == 2)
	sendCommand2LCD(0xC0 + b);
}
/* Initialize the LCD with predefined sequence */
void initLCD(void)
{
	LCD_CTRL_PORT_DATA &= ~(1<<RS_PIN);
	LCD_CTRL_PORT_DATA &= ~(1<<E_PIN);
	_delay_ms(20);
	/*--Reset process from data sheet--*/
	sendCommand2LCD(0x30);
	_delay_ms(5);
	sendCommand2LCD(0x30);
	_delay_ms(1);
	sendCommand2LCD(0x30);	
	_delay_ms(10);
	/*---------------------------------*/
	sendCommand2LCD(0x38);    //function set
	sendCommand2LCD(0x0C);    //display on,cursor off,blink off
	sendCommand2LCD(0x01);    //clear display
	sendCommand2LCD(0x06);    //entry mode, set increment
}

void init_timer1_comp_interrupt(void)
{
	TCNT1 = 0x00;
	TIMSK1 |= 0x02;
	TCCR1A &= 0xFC; TCCR1B |= 0x08; TCCR1B &= 0xEF;
	TCCR1B |= 0x05; TCCR1B &= 0xFC;
	OCR1A = 15625;
}

ISR (TIMER1_COMPA_vect)
{
	count = count + 250;
	displayValue(count);
}