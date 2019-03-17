/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #5  Exercise #1
 *    Exercise Description: 
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
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
	DDRA = 0x00; PORTA = 0xFF; //PORTA as input
	DDRB = 0xFF; PORTB = 0x00; //PORTB as output
	unsigned char lvl = 0x00;
	unsigned char input = 0x00;
	unsigned char tmpA = 0x00;
	MCUCR = 0x80;
	MCUCR = 0x80;
	while (1)
	{
		
		input = ~PINA;
		
		if(input >= 0x01){
			lvl = SetBit(lvl, 5, 1);
		}
		else{
			lvl = SetBit(lvl, 5, 0);
		}
		if(input >= 0x03){
			lvl = SetBit(lvl, 4, 1);
		}
		else{
			lvl = SetBit(lvl, 4, 0);
		}
		if(input >= 0x05){
			lvl = SetBit(lvl, 3, 1);
		}
		else{
			lvl = SetBit(lvl, 3, 0);
		}
		if(input >= 0x07){
			lvl = SetBit(lvl, 2, 1);
		}
		else{
			lvl = SetBit(lvl, 2, 0);
		}
		if(input >= 0x0A){
			lvl = SetBit(lvl, 1, 1);
		}
		else{
			lvl = SetBit(lvl, 1, 0);
		}
		if(input >= 0x0D){
			lvl = SetBit(lvl, 0, 1);
		}
		else{
			lvl = SetBit(lvl, 0, 0);
		}
		if(input <= 0x04){
			lvl = SetBit(lvl, 6, 1);
		}
		else{
			lvl = SetBit(lvl, 6, 0);
		}
		PORTB = lvl;



	}
}


