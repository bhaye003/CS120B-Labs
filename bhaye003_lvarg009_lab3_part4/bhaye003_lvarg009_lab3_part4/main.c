/*
 * bhaye003_lvarg009_lab3_part4.c
 *
 * Created: 1/17/2019 12:41:09 AM
 * Author : Bradley
 */ 

#include <avr/io.h>
/*(Challenge): Read an 8-bit value on PA7..PA0 and write that 
value on PB3..PB0PC7..PC4. 
That is to say,  take the upper nibble of PINA and map 
it to the lower nibble of PORTB,likewise take
the lower nibble of PINA and map it to the upper nibble 
of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
*/

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTB = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		for(unsigned char i = 0x00; i < 0x04; ++i){
			PORTC = SetBit(PORTC, i + 0x04, GetBit(PINA, i));
			PORTB = SetBit(PORTB, i, GetBit(PINA, i+ 0x04)); 
		}
		
    }
}

