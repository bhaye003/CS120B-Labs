/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #2  Exercise #3
 *    Exercise Description: Extension of Exercise #2... if all lots are taken, PC7 lights up.    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>
int main(void)
{
		DDRA = 0x00; PORTA = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		unsigned char tempA = 0x00;
		unsigned char cntavail;
    while (1) 
    {
		
		tempA = PINA & 0x0F;
		cntavail = ((tempA >> 3) & 0x01) + ((tempA >> 2) & 0x01) + ((tempA >> 1) & 0x01) + ((tempA) & 0x01);
		cntavail = 0x04 - cntavail;
		if(cntavail == 0x00){
			PORTC = 0x80;
		}
		else{PORTC = cntavail;
		}
    }
}


