/*
 * bhaye003_lvarg009_lab3_part5.c
 *
 * Created: 1/17/2019 12:59:57 AM
 * Author : Bradley
 */ 

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x01;
	
	unsigned short total = 0x0000;
	unsigned char tempB = 0x00;
	unsigned char tempD = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		total = 0x0000;
		tempB = PINB;
		tempD = PIND;
		
		total = tempD;
		total = total << 1;
		total = total + GetBit(tempB, 0);
		
		if(total >= 0x46){
			tempB = SetBit(tempB, 1, 1);
			tempB = SetBit(tempB, 2, 0);
		}
		else if(total >= 0x05){
			tempB = SetBit(tempB, 2, 1);
			tempB = SetBit(tempB, 1, 0);
		}
		else{
			tempB = 0x00;
		}
		PORTB = tempB;
		
    }
}

