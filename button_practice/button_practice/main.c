/*
 * button_practice.c
 *
 * Created: 3/14/2019 2:17:57 AM
 * Author : Bradley
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Replace with your application code */
	
	
    while (1) 
    {
		unsigned char input = ~PINA & 0x04;
		if(input){
			PORTC = 0xFF;
			PORTD = 0xE0;
		}else{
			PORTC = 0x00;
			PORTD = 0xFF;
		}
	}
}

