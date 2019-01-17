/*
 * bhaye003_lvarg009_lab3_part2.c
 *
 * Created: 1/16/2019 10:04:25 PM
 * Author : Bradley
 */ 

#include <avr/io.h>




int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		if(PINA >= 0x0D){
			PORTB = 0x3F; // 0011 1111
		}
		else if(PINA >= 0x0A){
			PORTB = 0x3E; // 0011 1110
		}
		else if(PINA >= 0x07){
			PORTB = 0x3C; //0011 1100
		}
		else if(PINA >= 0x05){
			PORTB = 0x38; //0011 1000
		}
		else if(PINA >= 0x03){
			PORTB = 0x70; // 0111 0000
		}
		else if(PINA >= 0x01){
			PORTB = 0x60; // 0110 0000
		}
		else{
			PORTB = 0x40; //0100 0000
		}
		
    }
}

