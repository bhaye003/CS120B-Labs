/*
 * bhaye003_lvarg009_lab10_part3.c
 *
 * Created: 2/13/2019 9:48:30 PM
 * Author : Bradley
 */ 

#include <avr/io.h>
#include "timer.h"


enum S_States{S_Start, S_ON, S_OFF} S_State;
enum TL_States{TL_Start, LED1, LED2, LED3} TL_State;
enum BL_States{BL_Start, Wait, LED4} BL_State;
enum C_States{C_Start, C_ON} C_State;

	

unsigned char three = 0x00;
unsigned char blink = 0x00;
unsigned char speak = 0x00;
unsigned char tempc = 0x00;



void set_PWM(double frequency){
	static double current_frequency;
	
	if(frequency != current_frequency){
		if(!frequency){ TCCR0B &= 0x08;}
		else { TCCR0B |= 0x03; }
		
		if(frequency < 0.954){OCR0A = 0xFFFF;}
		
		else if(frequency > 31250){OCR0A = 0x0000;}
		
		else{OCR0A = (short)(8000000 / (128 * frequency)) - 1;}
		
		TCNT0 = 0; //reset counter
		current_frequency = frequency;
	}
	
}
void PWM_on(){
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 < CS00);
	
	set_PWM(0);
}

void PWM_off(){
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


void TL_Tick(){
	switch(TL_State){
		case TL_Start:
		TL_State = LED1;
		break;
		case LED1:
		TL_State = LED2;
		break;
		case LED2:
		TL_State = LED3;
		break;
		case LED3:
		TL_State = LED1;
		break;
		default:
		break;
	}
	switch(TL_State){
		case TL_Start:
		break;
		case LED1:
		three = 0x01;
		break;
		case LED2:
		three = 0x02;
		break;
		case LED3:
		three = 0x04;
		break;
		default:
		break;
	}
	
}

void BL_Tick(){
	switch(BL_State){
		case BL_Start:
		BL_State = LED4;
		break;
		case Wait:
		BL_State = LED4;
		break;
		case LED4:
		BL_State = Wait;
		break;
		default:
		break;
		
	}
	switch(BL_State){
		case BL_Start:
		break;
		case Wait:
		blink = 0x00;
		break;
		case LED4:
		blink = 0x08;
		break;
		default:
		break;
	}
}
void S_Tick(){

	switch(S_State){
		case S_Start:
			S_State = S_ON;
			break;
		case S_ON:
			S_State = S_OFF;
			break;
		case S_OFF:
			S_State = S_ON;
			break;
		default:
			break;
	}
	switch(S_State){
		case S_Start:
		    speak = 0x00;
			break;
		case S_ON:
			speak = 0x00;
			set_PWM(260.0);
			break;
		case S_OFF:
			speak = 0x00;
			break;
		default:
			break;
	}
}

void C_Tick(){
	switch(C_State){
		case C_Start:
		C_State = C_ON;
		break;
		case C_ON:
		C_State = C_ON;
		break;
		default:
		break;
	}
	switch(C_State){
		case C_Start:
		break;
		case C_ON:
		tempc = blink | three | speak;
		break;
		default:
		break;
		
	}
	PORTB = tempc;
	
	
	
}





int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned long BL_elapsedTime = 0;
	unsigned long TL_elapsedTime = 0;
	unsigned long S_elapsedTime = 0;
	const unsigned long timerPeriod = 2;
	
	
	TimerSet(2);
	TimerOn();
	PWM_on();
	BL_State = BL_Start;
	TL_State = TL_Start;

	/* Replace with your application code */
	while (1)
	{
		unsigned char button = ~PINA & 0x04;
		if(BL_elapsedTime >= 1000){
			BL_Tick();
			BL_elapsedTime = 0;
		}
		if(TL_elapsedTime >= 300){
			TL_Tick();
			TL_elapsedTime = 0;
		}
		if(S_elapsedTime >= 2 && button){
			S_Tick();

			S_elapsedTime = 0;
		}
		
		C_Tick();
		
		while(!TimerFlag){}
		TimerFlag = 0;
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
		S_elapsedTime += timerPeriod;
	}
}


