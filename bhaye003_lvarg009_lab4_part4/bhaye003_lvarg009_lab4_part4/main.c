/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #4  Exercise #4
 *    Exercise Description:
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */


#include <avr/io.h>

enum CL_States {CL_Start, CL_Enter, CL_Press, CL_Release, CL_Open, CL_LockP, CL_LockR} CL_State;

void CL_Tick(){
		switch(CL_State){
			case CL_Start:
				CL_State = CL_Enter;
				break;
			case CL_Enter:
				if(PINA == 0x04){
					CL_State = CL_Press;
				}
				break;
			case CL_Press:
				if(PINA == 0x00){
					CL_State = CL_Release;
				}
				break;
			case CL_Release:
				if(PINA == 0x02){
					CL_State = CL_Open;
				}
				else if(PINA == 0x00){
					CL_State = CL_Release;
				}
				else{
					CL_State = CL_Enter;
				}
				break;
			case CL_Open:
				if(PINA == 0x80){
					CL_State = CL_Enter;
				}
				if(PINA == 0x04){
					CL_State = CL_LockP;
				}
				break;
			
			case CL_LockP:
				if(PINA == 0x00){
					CL_State = CL_LockR;
				}
				break;
			case CL_LockR:
				if(PINA == 0x02){
					CL_State = CL_Enter;
				}
				else if(PINA == 0x00){
					CL_State = CL_LockR;
				}
				else{
					CL_State = CL_Open;
				}
				break;
			default:
				break;	
		}

		switch(CL_State){
			case CL_Start:
				break;
			case CL_Enter:
			PORTB = 0x00;
				break;
			case CL_Press:
				break;
			case CL_Release:
				break;
			case CL_Open:
			PORTB = 0x01;
				break;
			case CL_LockP:
				break;
			case CL_LockR:
				break;
			default:
				break;
		}
}
int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		CL_Tick();
    }
}
