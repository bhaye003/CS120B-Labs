/*
 * bhaye003_lvarg009_lab3_part1.c
 *
 * Created: 1/15/2019 4:14:27 PM
 * Author : Bradley
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as outputs,
	DDRA = 0xFF; PORTA = 0x00;
	unsigned char tmpA = PINA;
	unsigned char tmpB = PINB;
	unsigned char cntA = 0x00;
	unsigned char cntB = 0x00;	
    /* Replace with your application code */
    while (1) 
    {		
		unsigned char cntA = 0x00;
		unsigned char cntB = 0x00;
		for(unsigned char  i = 0x00; i <= 0x07; ++i){
			cntA = cntA + GetBit(tmpA, i);
			cntB = cntB + GetBit(tmpB, i);
		}
		PORTC = cntA + cntB;
		
    }
}

