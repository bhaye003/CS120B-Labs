/*
 * Matrix_practice.c
 *
 * Created: 3/7/2019 1:57:40 PM
 * Author : Bradley
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
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




//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Demo_States {shift};
int Demo_Tick(int state) {
	// Local Variables
	static unsigned char pattern = 0x01; // LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xE0; // Row(s) displaying pattern.
	// 0: display pattern on row
	// 1: do NOT display pattern on row

	// Transitions
	switch (state) {
		case shift:
		break;
		default:
		state = shift;
		break;

	}
	// Actions
	switch (state) {
		case shift:
		if (pattern == 0x01) { // Reset demo
		
			pattern = 0x80;
		}
		//else{ // Move LED to start of next row
			//row = (row << 1) | 0x01;
		//}
		else { // Shift LED one spot to the right on cur row
			pattern >>= 1;
		} 
		break;
		default:
		break;

	}
	PORTC = pattern; // Pattern to display
	PORTD = row; // Row(s) displaying pattern
	return state;
}


int main(void) {
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD =0x00;
	
	TimerSet(100);
	TimerOn(); 
	int state = shift;
	while (1) {
		Demo_Tick(state);      // Execute one synchSM tick
		while (!TimerFlag){}  // Wait for BL's period
		TimerFlag = 0;        // Lower flag
	}
}

