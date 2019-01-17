/*
 * bhaye003_lvarg009_lab3_part2.c
 *
 * Created: 1/16/2019 10:04:25 PM
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		if((PINA & 0x0F) >= 0x0D){
			PORTC = 0x3F; // 0011 1111
		}
		else if((PINA & 0x0F) >= 0x0A){
			PORTC = 0x3E; // 0011 1110
		}
		else if((PINA & 0x0F) >= 0x07){
			PORTC = 0x3C; //0011 1100
		}
		else if((PINA & 0x0F) >= 0x05){
			PORTC = 0x38; //0011 1000
		}
		else if((PINA & 0x0F) >= 0x03){
			PORTC = 0x70; // 0111 0000
		}
		else if((PINA & 0x0F) >= 0x01){
			PORTC = 0x60; // 0110 0000
		}
		else if((PINA & 0x0F) == 0x00){
			PORTC = 0x40; //0100 0000
		}
		if(GetBit(PINA,4)){
			if(GetBit(PINA,5)){
				if((!GetBit(PINA,6))){
					PORTC = SetBit(PORTC,7,1);
				}
			}
		}

    }
}