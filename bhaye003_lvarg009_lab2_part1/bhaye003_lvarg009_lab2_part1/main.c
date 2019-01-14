/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #2  Exercise #1
 *    Exercise Description: PB0 will display in only the case that input A is 0x01.
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */ 


#include <avr/io.h>


int main(void)
{
	/* Replace with your application code */
	while (1)
	{
		DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs

		DDRB = 0xFF; // Configure port B's 8 pins as outputs
		PORTB = 0x00; // Initialize PORTB output to 0’s
		unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
		unsigned char tmpA = 0x00; // Temporary variable to hold the value of A

		while(1)
		{
			tmpA = (PINA & 0x03); //Gets input and masks all but pins A0 and A1
			tmpB = (tmpA == 0x01);
			PORTB = tmpB;
		}

	}
}
