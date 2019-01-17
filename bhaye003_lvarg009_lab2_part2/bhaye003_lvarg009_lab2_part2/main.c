/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #2  Exercise #2
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
	unsigned char tempA = 0x00;
	unsigned char cntavail;
	while (1)
	{
		
		tempA = PINA & 0x0F;
		cntavail = ((tempA >> 3) & 0x01) + ((tempA >> 2) & 0x01) + ((tempA >> 1) & 0x01) + ((tempA) & 0x01);
		cntavail = 0x04 - cntavail;
		PORTC = cntavail;
	}
}


