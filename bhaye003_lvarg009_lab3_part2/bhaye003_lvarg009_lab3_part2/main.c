 /*Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #3  Exercise #2
 *    Exercise Description: PORT C displays the number of remaining parking spots, each of which are indicated by PC3 - PC0.
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>




int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		if(PINA >= 0x0D){
			PORTC = 0x3F; // 0011 1111
		}
		else if(PINA >= 0x0A){
			PORTC = 0x3E; // 0011 1110
		}
		else if(PINA >= 0x07){
			PORTC = 0x3C; //0011 1100
		}
		else if(PINA >= 0x05){
			PORTC = 0x38; //0011 1000
		}
		else if(PINA >= 0x03){
			PORTC = 0x70; // 0111 0000
		}
		else if(PINA >= 0x01){
			PORTC = 0x60; // 0110 0000
		}
		else{
			PORTC = 0x40; //0100 0000
		}
		
    }
}

