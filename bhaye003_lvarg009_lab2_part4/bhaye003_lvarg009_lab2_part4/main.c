/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #2  Exercise #4
 *    Exercise Description: *Challenge* 
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>



int main(void)
{
	MCUCR = 0x80;
	MCUCR = 0x80;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
    while (1) 
    {
		if(PINA + PINB + PINC > 0x8C){
			PORTD = 0x01;
			if((PINA - PINC > 0x50) || (PINC - PINA > 0x50)){
				PORTD = 0x03;
			}		
		}
		else{
			if((PINA - PINC > 0x50) || (PINC - PINA > 0x50)){
				PORTD = 0x02;
			}
		}
	
	}
}


