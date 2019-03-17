/*
 * bhaye003_lvarg009_lab6_part1.c
 *
 * Created: 1/31/2019 3:58:08 PM
 * Author : Bradley
 */ 
#include<avr/interrupt.h>
#include <avr/io.h>

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


enum States{Start, S_B0, S_B1, S_B2}state;

void Tick(){
	switch(state){
		case Start:
			state = S_B0;
			break;
		case S_B0:
			while(!TimerFlag);
			TimerFlag = 0;
			state = S_B1;
			break;
		case S_B1:
			while(!TimerFlag);
			TimerFlag = 0;
			state = S_B2;
			break;
		case S_B2:
			while(!TimerFlag);
			TimerFlag = 0;
			state = S_B0;
			break;
		default:
			break;
	}
	switch(state){
		case Start:
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
		default:
			break;
	}
}
		




int main(void)
{
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	TimerSet(100);
	TimerOn();

    /* Replace with your application code */
    while (1) 
    {
		Tick();
		//tmpB = ~tmpB;
		//PORTB = tmpB;
		//while(!TimerFlag);
		//TimerFlag = 0;
		PORTB = tmpB;
    }
}

