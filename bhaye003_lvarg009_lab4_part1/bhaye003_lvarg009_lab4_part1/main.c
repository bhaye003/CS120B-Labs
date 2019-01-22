/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #2  Exercise #2
 *    Exercise Description: PORT C displays the number of remaining parking spots, each of which are indicated by PC3 - PC0.
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */


#include <avr/io.h>

enum PB_States {PB_Start, PB_PB0, PB_PB1} PB_State;

void PB_Tick(){
	switch(PB_State){
		case PB_Start:
			PB_State = PB_PB0;
			break;
		case PB_PB0:
			if(PINA == 0x01){
				PORTB = PORTB << 1;
				PB_State = PB_PB1;
			}
			else{
				PB_State = PB_PB0;
			}
			break;
		case PB_PB1:
			if(PINA == 0x01){
				PORTB = PORTB >> 1;
				PB_State = PB_PB0;
			}
			else{
				PB_State = PB_PB1;
			}
			break;
		default:
			PB_State = PB_Start;
			break;
	}
	switch(PB_State){
		case PB_Start:
			break;
			
		case PB_PB0:
			break;
			
		case PB_PB1:
			break;
		default:
			break;
	}
}

int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x01;
	PB_State = PB_Start;
    /* Replace with your application code */
    while (1) 
    {
		PB_Tick();
    }
}

