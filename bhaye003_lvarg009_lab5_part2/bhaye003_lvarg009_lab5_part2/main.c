/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #5  Exercise #2
 *    Exercise Description: 
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
*/

#include <avr/io.h>

enum States{Start, Init, Waits0, Waits1, s0, s1, twoBtn} state;

unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char tempB = 0x00;

void Tick(){
	button1 = ~PINA & 0x01;
	button2 = ~PINA & 0x02;
	tempB = PORTB;
	
	switch(state){
		case Start:
		tempB = 0x00;
		state = Init;
		break;
		
		
		case Init:
		if(button1 && button2){
			state = twoBtn;
			}
		
		else if(button1){
			state = Waits0;
			}
		
		else if(button2){
			state = Waits1;
			}
		
		else if(!button1 && !button2){
			state = Init;
		}
		break;
		
		
		case Waits0:
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
		
		
		case s0:
		state = Init;
		break;
		
		
		case s1:
		state = Init;
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
		
		case Waits0:
		break;
		
		case Waits1:
		break;
		
		case s0:
		if(tempB < 9){
			tempB++;
		}
		break;
		
		case s1:
		if(tempB > 0){
			tempB--;
		}
		break;
		
		case twoBtn:
		tempB = 0;
		break;
		
		default:
		break;
	}
	PORTB = tempB;
}

int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	tempB = 0x00;
	state = Start;
	while (1)
	{
		Tick();
	}
}