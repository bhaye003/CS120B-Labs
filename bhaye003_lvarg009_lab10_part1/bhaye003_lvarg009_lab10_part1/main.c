/*
 * bhaye003_lvarg009_lab10_part1.c
 *
 * Created: 2/12/2019 4:09:01 PM
 * Author : Bradley
 */ 

#include <avr/io.h>
#include "timer.h"


enum TL_States{TL_Start, LED1, LED2, LED3} TL_State;
enum BL_States{BL_Start, Wait, LED4} BL_State;
enum C_States{C_Start, C_ON} C_State;

unsigned char three = 0x00;
unsigned char blink = 0x00;
unsigned char tempc = 0x00;

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
			BL_State = Wait;
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
			TL_Tick();
			BL_Tick();
			tempc = blink | three;
			break;
		default:
			break;
		
	}
	PORTB = tempc;
	
	
	
}

	



int main(void)
{
	//DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
			
	tempc = 0;
	TimerSet(1000);
	TimerOn();
	BL_State = BL_Start;
	TL_State = TL_Start;

    /* Replace with your application code */
    while (1) 
    {
		C_Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
		
    }
}

