/*
 * Joystick.c
 *
 * Created: 2/28/2019 1:10:28 PM
 * Author : Bradley
 */ 


#include "io.c"
#include <stdlib.h>
#include <string.h>
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

enum SR_States {shift};
int ShiftRightTick(int state){
		
		static unsigned pattern = 0x80;
		
		// Transitions
		switch (state) {
			case shift:
			break;
			default:
			state = shift;
			break;

		}
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


	};


int main(void)
{
	PORTA = 0xFF; DDRA = 0x00;
	PORTB = 0x00; DDRB = 0xFF;
	PORTC = 0x00; DDRC = 0xFF;
	PORTD = 0x00; DDRD = 0xFF;
	
	static unsigned pattern = 0xFF;
	static unsigned row = 0xFE;
	
	adc_init();
	
	int16_t x,y;
	
	TimerSet(100);
	TimerOn();
	int RS_State = shift;
	
	unsigned char *str, *str2;
	str = (unsigned char*) malloc(sizeof(unsigned char) * 5);
	str2 = (unsigned char*) malloc(sizeof(unsigned char) * 5);
	//unsigned char result = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		x = adc_read(0);
		y = adc_read(1);
		
		PORTC = 0x10;
		PORTD = 0xFB;
		
		itoa(x, str, 10);
		if(x >= 1000){
		PORTC = 0x08;
		//	result = 0x01;

		
		}
		else if(x < 200){
			PORTC = 0x20;
		//	result = 0x02;
		}
		
		itoa(y, str2, 10);
		if(y >= 900){
			PORTD = 0xFD;
		//	result = 0x04;
		}
		else if(y < 100){
			PORTD = 0xF7;
		//	result= 0x08;
		}
		//PORTB = result;
    }
}

