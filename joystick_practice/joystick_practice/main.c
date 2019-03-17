#include <avr/io.h>

//header to enable data flow control over pins

#define F_CPU 1000000

//telling controller crystal frequency attached

#include <util/delay.h>

//header to enable delay function in program



#define    E   5

//giving name “enable”  to 5th pin of PORTD, since it Is connected to LCD enable pin

#define RS  6

//giving name “registerselection” to 6th pin of PORTD, since is connected to LCD RS pin



void send_a_command(unsigned char command);

void send_a_character(unsigned char character);

void send_a_string(char *string_of_characters);

int main(void)

{

	DDRB = 0xFF;

	//putting portB and portD as output pins

	DDRD = 0xFF;

	_delay_ms(50);//giving delay of 50ms

	DDRA = 0;//Taking portC as input.

	

	int HORIZONTAL= 512;//neutral value on x-axis

	int VERTICAl = 512;// neutral value on y-axis

	int HORIZONTALMOV =0;

	int VERTICAlMOV =0;

	

	ADMUX |=(1<<REFS0);//setting the reference of ADC

	

	ADCSRA |=(1<<ADEN) |(1<ADPS2)|(1<ADPS1) |(1<<ADPS0);

	//enabling the ADC,, setting prescalar 128

	

	while(1)

	{

		switch (ADMUX)//changing between channels by switch statement

		{

			case 0x40://When ADMUX==0x40

			{

				ADCSRA |=(1<<ADSC);//start ADC conversion

				while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion

				HORIZONTALMOV = ADC;//moving value

				ADC=0;//reset ADC register

				ADMUX=0x41;//changing channel

				break;

			}

			case 0x41:

			{

				ADCSRA |=(1<<ADSC);// start ADC conversion

				while ( !(ADCSRA & (1<<ADIF)));// wait till ADC conversion

				VERTICAlMOV = ADC;// moving value

				ADC=0;// reset ADC register

				ADMUX=0x40;// changing channel

				break;

			}

		}

		

		if (HORIZONTALMOV<HORIZONTAL-50)

		{

			PORTD |=(1<<PIND3);

			_delay_ms(5);

			PORTD=0;

		}

		

		

		if (HORIZONTALMOV>(HORIZONTAL+50))

		{

			PORTD |=(1<<PIND2);

			_delay_ms(5);

			PORTD=0;

		}

		

		

		if (VERTICAlMOV<VERTICAl-50)

		{

			PORTD |=(1<<PIND1);

			_delay_ms(5);

			PORTD=0;

		}

		if (VERTICAlMOV>VERTICAl+50)

		{

			PORTD |=(1<<PIND0);

			_delay_ms(5);

			PORTD=0;

		}

	}
}


