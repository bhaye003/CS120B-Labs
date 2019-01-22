/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #4  Exercise #2
 *    Exercise Description: PORT C displays the number of remaining parking spots, each of which are indicated by PC3 - PC0.
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */


#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


enum LED_States {LED_Start, WaitRise, Increment, Decrement, Reset} LED_State;
	
void LED_Tick(){
	switch(LED_State){
		case LED_Start:
			LED_State = WaitRise;
			PORTC = 0x07;
			break;
		
		case WaitRise:
			if(GetBit(PINA, 0)&&(!(GetBit(PINA, 1)))){
				LED_State = Increment;
				

			}
			else if((!GetBit(PINA, 0))&&(GetBit(PINA, 1))){
				LED_State = Decrement;
			}
			else if(GetBit(PINA, 0)&& GetBit(PINA, 1)){
				LED_State = Reset;
			}
			break;
			
		case Increment:
			LED_State = WaitRise;
			break;
		case Decrement:
			LED_State = WaitRise;
			break;
		case Reset:
			LED_State = WaitRise;
			break;
		default:
			break;
			
	}
	
	switch(LED_State){
		case LED_Start:
			break;
		case WaitRise:
			break;
		case Increment:
			if(PORTC < 9){
				PORTC = PORTC + 1;
			}
			break;
		case Decrement:
			if(PORTC > 0){
				PORTC = PORTC - 1;
			}
			break;
		case Reset:
			PORTC = 0x00;
			break;
			
	}
	
	
	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	LED_State = LED_Start;
    /* Replace with your application code */
    while (1) 
    {
		LED_Tick();
    }
}

