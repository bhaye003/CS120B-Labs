/*
 * bhaye003_lvarg009_lab7_part1.c
 *
 * Created: 2/2/2019 4:51:46 PM
 * Author : Bradley
 */ 

#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;    // Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum States{Start, Init, s0, s1, twoBtn}state;

unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char tempB = 0x00;

void Tick(){
	button1 = ~PINA & 0x01;
	button2 = ~PINA & 0x02;
	
	switch(state){
		case Start:
		tempB = 0;
		state = Init;
		break;
		
		
		case Init:
		if(button1 && button2){
			state = twoBtn;
		}
		
		else if(button1){
			state = s0;
		}
		
		else if(button2){
			state = s1;
		}
		
		else if(!button1 && !button2){
			state = Init;
		}
		break;
		
		
		/*case Waits0:
		if(button2){
			state = twoBtn;
			break;
		}
		else if(!button1){
			state = s0;
		}
		break;
		
		case Waits1:
		if(button1){
			state = twoBtn;
			break;
		}
		if(!button2){
			state = s1;
		}
		break;
		*/
		
		case s0:
		if(button1){
			state = s0;
		}
		else if(!button1){
			state = Init;
		}
		break;
		
		
		case s1:
		if(button2){
			state = s1;
		}
		else if(!button2){
		state = Init;
		}
		break;
		
		
		case twoBtn:
		if(!button1 && !button2){
			state = Init;
			}else{
			state = twoBtn;
		}
		break;
		
		
		default:
		state = Start;
		break;
	}
	switch(state){
		case Start:
		break;
		
		case Init:
		break;
		
		/*case Waits0:
		break;
		
		case Waits1:
		break;
		*/
		case s0:
		if(tempB < 9){
			tempB++;
		//LCD_WriteData( tempB + '0' );
		}
		break;
		
		case s1:
		if(tempB > 0){
			tempB--;
		//LCD_WriteData( tempB + '0');
		}
		break;
		
		case twoBtn:
		tempB = 0;
		//LCD_WriteData( tempB + '0');
		break;
		
		default:
		break;
	}
	LCD_Cursor(1);
	LCD_WriteData(tempB + '0');
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	TimerSet(1000);
	TimerOn();
	
	
	tempB = 0x00;
	state = Start;
	
	
	// Initializes the LCD display
	LCD_init();
	
	
	while(1) {
		Tick();
		while (!TimerFlag);    // Wait 1 sec
		TimerFlag = 0;

	}
}
