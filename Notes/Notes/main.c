/*
* Notes.c
*
* Created: 3/12/2019 4:18:38 PM
* Author : Bradley
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"
#include<avr/eeprom.h>


void adc_init(){
	ADMUX = (1 << REFS0);
	
	ADCSRA = (1 << ADEN)|(1 << ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

void CreateCustomCharacter(unsigned char *Pattern, const char Location){
	int i = 0;
	LCD_WriteCommand(0x40+(Location*8));
	for(i = 0; i < 8; i++){
		LCD_WriteData(Pattern [i]);
	}
}

uint16_t adc_read(uint8_t ch){
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return(ADC);
}

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTB		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
	LCD_WriteCommand(0x01);
}

void LCD_init(void) {

	//wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
}

void LCD_WriteCommand (unsigned char Command) {
	CLR_BIT(CONTROL_BUS,RS);
	DATA_BUS = Command;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
	SET_BIT(CONTROL_BUS,RS);
	DATA_BUS = Data;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
	//LCD_ClearScreen();
	unsigned char c = column;
	while(*string) {
		LCD_Cursor(c++);
		LCD_WriteData(*string++);
	}
}

void LCD_Cursor(unsigned char column) {
	if ( column < 17 ) { // 16x1 LCD: column < 9
		// 16x2 LCD: column < 17
		LCD_WriteCommand(0x80 + column - 1);
		} else {
		LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
		// 16x2 LCD: column - 9
	}
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
	int i,j;
	for(i=0;i<miliSec;i++)
	for(j=0;j<775;j++)
	{
		asm("nop");
	}
}

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;

void TimerOn() {

	
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOFF() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}



enum NStates{init, start, up, down, left, right, GameOver} NState;
void Note_Tick(){
	static unsigned pattern = 0x00;
	static unsigned row = 0x00;
	static unsigned int count;
	static unsigned int inicount = 15;
	static unsigned int score = 0;
	char sOut[3];
	char sHigh[3];
	int16_t x = adc_read(0);
	int16_t y = adc_read(1);
	unsigned int stick = 0;
	unsigned int wrong = 0;
	unsigned char* address = 0x21;
	unsigned char button = ~PINA & 0x04;
	unsigned char Pattern1 [ ] = {0x0E, 0x1F, 0x15, 0x1B, 0x0E, 0x0E, 0x0A};
	//Transitions
	switch(NState){
		case init:
		if(button){
			NState = init;
			break;
		}
		else{
			if(inicount == 0){
				inicount = 15;
				LCD_ClearScreen();
				NState = start;
			}
			else{
				NState = init;
			}
		}
		break;
		case start:
			if(button){
				LCD_ClearScreen();
				NState = init;
				break;
			}
			else{
				NState = up;
				break;
		case up:
			if(y >= 900){
				stick = 1;
			}
			else{
				wrong = 1;
			}
			if(count > 0){
				NState = up;
				break;
			}
			else{
				if(wrong == 1){
					NState = GameOver;
					break;
				}
			if(stick == 1){
					score++;
					stick = 0;
			}
					//score++;
			NState = down;
			break;
				}
			}
		case down:
		if(button){
			LCD_ClearScreen();
			NState = init;
			break;
		}
		else{
			if(y < 100){
				stick = 1;
			}
			else{
				wrong = 1;
			}
			if(count > 0){
				NState = down;
				break;
			}
			else{
				if(wrong == 1){
					NState = GameOver;
					break;
				}
				if(stick == 1){
					score++;
					stick = 0;
				}
				NState = left;
				break;
			}
		}
		case left:
		if(button){
			LCD_ClearScreen();
			NState = init;
			break;
		}
		else{
			if(x < 150){
				stick = 1;
			}	
			else{
				wrong = 1;
			}		
			if(count > 0){
				NState = left;
				break;
			}
			else{
				if(wrong == 1){
					NState = GameOver;
					break;
				}			
				if(stick == 1){
				score++;
				stick = 0;
				}
				NState = right;
				break;
			}
		}
		case right:
		if(button){
			LCD_ClearScreen();
			NState = init;
		}
		else{
				if(x >= 1000){
					stick = 1;
				}
				else{
					wrong = 1;
				}
				if(count > 0){
					NState = right;
					break;
				}
				else{
					if(wrong == 1){
						NState = GameOver;
						break;
					}
					if(stick == 1){
						score++;
						stick = 0;
					}
					NState = up;
					break;
				}
		}
		case GameOver:
			if(button){
				LCD_ClearScreen();
				NState = init;
				break;
			}
			//else{
			//	NState = GameOver;
			//	break;
			//}
			break;
		default:
			NState = init;
		break;
		
	}
	//State Actions
	switch(NState){
		case init:
			score = 0;
			if(inicount > 0){
				LCD_ClearScreen();
				LCD_DisplayString(1, "Get Ready!!!");
				inicount--;
			}

			break;
		case start:
		row = 0xEF;
		pattern = 0xFF;
		count = 4;
		LCD_DisplayString(17, "High Score:");
		itoa(eeprom_read_byte(address), sHigh, 10);
		LCD_DisplayString(30, sHigh);
		break;
		case up:
		if(count == 0)
		{
			count = 4;
			row = 0xEF;
			pattern = 0xFF;
			//score++;
			break;
		}else
		{
			itoa(score, sOut,  10);
			LCD_DisplayString(1, "Score:");
			LCD_DisplayString(8, sOut);
			LCD_DisplayString(17, "High Score:");
			itoa(eeprom_read_byte(address), sHigh, 10);
			LCD_DisplayString(30, sHigh);
			row = (row >> 1) | 0xE0;
			count--;
			break;
		}
		case down:
		if(count == 0)
		{
			count = 4;
			row = 0xFE;
			pattern = 0xFF;
			//score++;
			break;
		}else
		{
			itoa(score, sOut,  10);
			LCD_DisplayString(1, "Score:");
			LCD_DisplayString(8, sOut);
			LCD_DisplayString(17, "High Score:");
			itoa(eeprom_read_byte(address), sHigh, 10);
			LCD_DisplayString(30, sHigh);			
			row = (row << 1) | 0x01;
			count--;
			break;
		}
		case left:
		if(count == 0)
		{
			count = 7;
			row = 0xE0;
			pattern = 0x01;
			//score++;
			break;
		}else
		{
			itoa(score, sOut,  10);
			LCD_DisplayString(1, "Score:");
			LCD_DisplayString(8, sOut);
			LCD_DisplayString(17, "High Score:");
			itoa(eeprom_read_byte(address), sHigh, 10);
			LCD_DisplayString(30, sHigh);
			pattern <<= 1;
			count--;
			break;
		}
		case right:
		if(count == 0)
		{
			count = 7;
			row = 0xE0;
			pattern = 0x80;
			//score++;
			break;
		}else
		{	
			itoa(score, sOut,  10);
			LCD_DisplayString(1, "Score:");
			LCD_DisplayString(8, sOut);
			LCD_DisplayString(17, "High Score:");
			itoa(eeprom_read_byte(address), sHigh, 10);
			LCD_DisplayString(30, sHigh);
			pattern >>= 1;
			count--;
			break;
			
		}
		
		case GameOver:
			pattern = 0xFF;
			row = 0xE0;
			LCD_ClearScreen();
			itoa(score, sOut,  10);
			LCD_DisplayString(1, "Score:");
			LCD_DisplayString(8, sOut);
			LCD_DisplayString(17, " GAME OVER");
			CreateCustomCharacter(Pattern1, 1);
			LCD_WriteCommand(0x8A);
			LCD_WriteData(1);
			LCD_WriteCommand(0x8B);
			LCD_WriteData(1);
			LCD_WriteCommand(0x8C);
			LCD_WriteData(1);
			LCD_WriteCommand(0x8D);
			LCD_WriteData(1);
			break;	
	
		default:
		break;
		

	}
	if(score > eeprom_read_byte(address)){
		eeprom_write_byte(address, score);
	}
	
	
	//itoa(score, sOut,  10);
	//LCD_DisplayString(1, "Score:");
	//LCD_DisplayString(8, sOut);

	//LCD_DisplayString(1, "Stick:");
	//LCD_DisplayString(8, stick);
	
	
	PORTC = pattern;
	PORTD = row;
	
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char data = 0xFF;
	unsigned char button = ~PINA & 0x04;
	
	//eeprom_write_byte(0x21, 0);
	
	
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	//LCD_WriteData(data + '0');
	//LCD_DisplayString(1, "Hello World");
	//LCD_DisplayString(17, "High Score here");
	TimerSet(100);
	TimerOn();
	NState = init;
	
	adc_init();
	
	int16_t x,y;
	unsigned char *str, *str2;
	str = (unsigned char*) malloc(sizeof(unsigned char) * 5);
	str2 = (unsigned char*) malloc(sizeof(unsigned char) * 5);

	
	
	/* Replace with your application code */
	while (1)
	{
		//x = adc_read(0);
		//y = adc_read(1);
		//if(x >= 1000 || x < 200 || y >=900 || y < 100){
		//	PORTC = 0x50;
		//	PORTD = 0xFB;
		//}
		//else{
		//	PORTC = 0x50;
		//	PORTD = 0xF;
		//}
		Note_Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

