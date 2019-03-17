/*
 * bhaye003_lvarg009_lab7_part2.c
 *
 * Created: 2/2/2019 8:23:49 PM
 * Author : Bradley
 */ 
#include<avr/interrupt.h>
#include <avr/io.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;
unsigned char score = 0x05;


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



enum States{Start, S_B0, S_B1, S_B2, Press, Release, Winner}state;

void Tick(){
	unsigned char button = ~PINA & 0x01;
	switch(state){
		case Start:
			state = S_B0;
			break;
		case S_B0:
		if(button){
			if(score > 0x00){
				score--;
			}
		}

		else{
			state = S_B1;
		}
		break;
		case S_B1:
		if(button){
			score++;
			if(score == 0x09){
				state = Winner;
			}
			else{
				state = Press;
			}
		}
		else{
			state = S_B2;
		}
		break;
		case S_B2:
		if(button){
			if(score > 0){
				score--;
			}

		}
		else{
			state = S_B0;
		}
		break;
		
		case Press:
		if (button) {
			state = Press;
		}
		else if (!button) {
			state = Release;
		}
		break;
		case Release:
		if (button) {
			if(score == 0x09){
				state = Start;
			}
			else{
					state = S_B0;
			}

		}
		else if (!button) {
			state = Release;
		}
		break;
		
		case Winner:
			if(!button){
				state = Release;
			}
			else{
				state = Winner;
			}
		break;
		
		default:
			break;
	}
	
	switch(state){
		case Start:
		score = 0x05;
		break;
		
		case S_B0:
		tmpB = 0x01;
		break;
		
		case S_B1:
		tmpB = 0x02;
		break;
		
		case S_B2:
		tmpB = 0x04;
		break;

		case Winner:
			//LCD_ClearScreen();
			//LCD_Cursor(1);
			tmpB = 0x07;
		break;

		default:
		break;
	}
	PORTB = tmpB;
	
	if(score <= 0x09){
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(score + '0');
	}
}





int main(void)
{
	MCUCR = 0x80;
	MCUCR = 0x80;
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	TimerSet(300);
	TimerOn();
	LCD_init();
	
	state = Start;

	/* Replace with your application code */
	while (1)
	{
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;

	}
}
