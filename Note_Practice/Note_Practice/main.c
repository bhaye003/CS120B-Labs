/*
 * Note_Practice.c
 *
 * Created: 3/10/2019 2:29:08 PM
 * Author : Bradley
 */ 

#include <avr/io.h>
#include "io.c"
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

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



void adc_init(){
	ADMUX = (1 << REFS0);
	
	ADCSRA = (1 << ADEN)|(1 << ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch){
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return(ADC);
}






enum Demo_States {shift};

int Demo_Tick(int state) {
	// Local Variables
	static unsigned char pattern = 0x80; // LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE; // Row(s) displaying pattern.
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
		if (row == 0xEF && pattern == 0x01) { // Reset demo
			pattern = 0x80;
			row = 0xFE;
		}
		else if (pattern == 0x01) { // Move LED to start of next row
			pattern = 0x80;
			row = (row << 1) | 0x01;
		}
		else { // Shift LED one spot to the right on current row
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

int main(void){
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(200);
	TimerOn();
	int state = shift;
    
	int16_t x, y;
	
	unsigned char *str, *str2;
	str = (unsigned char*) malloc(sizeof(unsigned char) * 5);
	str2 = (unsigned char*) malloc(sizeof(unsigned char) * 5);

	
    while (1) 
    {
		x = adc_read(0);
		y = adc_read(1);
		
		PORTC = 0x20;
		PORTD = 0xFB;
		
		itoa(x, str, 10);
		if(x == 0){
			PORTC = 0x80;
			PORTD = 0xFE;
		}
		else if(x >= 1000){
			PORTD = 0x70;
			PORTC = 0xFE;
		}
		
		while(!TimerFlag){};
		TimerFlag = 0;
		
    }
}

