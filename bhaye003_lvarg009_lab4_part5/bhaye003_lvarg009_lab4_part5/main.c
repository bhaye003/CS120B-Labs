/*    Partner(s) Name & E-mail: Luis Vargas lvarg009@ucr.edu
 *    Lab Section: 24
 *    Assignment: Lab #4  Exercise #5
 *    Exercise Description: PORT C displays the number of remaining parking spots, each of which are indicated by PC3 - PC0.
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */


#include <avr/io.h>
enum CL_States {CL_Start, CL_Enter, CL_1st, CL_2nd, CL_3rd, CL_4th, CL_Flip} CL_State;

	char temp1 = 0x00;
	char temp2 = 0x00;
	char temp3 = 0x00;
	char temp4 = 0x00;

int compareArrays(char a[], char b[], int n) {
  int i;
	for(i = 0; i < n; i++) {
		if (a[i] != b[i]){
			return 0;
		}
	}
	return 1;
}

void CL_Tick(){
	char Solution[4] = {0x04, 0x01, 0x02, 0x01};
	char arr[4];

	switch(CL_State){
		case CL_Start:
			CL_State = CL_Enter;
		case CL_Enter:
			if(PINA != 0x00){
				CL_State = CL_1st;
				temp1 = PINA;
				arr[0] = temp1;
			}
			else{
				CL_State = CL_Enter;
			}
			break;
		case CL_1st:
			if(PINA != temp1){
				CL_State = CL_2nd;
				temp2 = PINA;
				arr[1] = temp2;
			}
			else{
				CL_State = CL_1st;
			}
			break;
		case CL_2nd:
			if(PINA != temp2){
				CL_State = CL_3rd;
				temp3 = PINA;
				arr[2] = temp3;
			}
			else{
				CL_State = CL_2nd;
			}
			break;
		case CL_3rd:
			if(PINA != temp3){
				CL_State = CL_4th;
				temp4 = PINA;
				arr[3] = temp4;
			}
			else{
				CL_State = CL_3rd;
			}
			break;
		case CL_4th:
			if(compareArrays(arr, Solution, 4)){
				CL_State = CL_Flip;
			}
			else{
				CL_State = CL_Enter;
			}
			break;
		case CL_Flip:
			CL_State = CL_Enter;
			break;
		default:
			break;
	}
	switch(CL_State){
		case CL_Start:
			break;
		case CL_Enter:
			if(PORTB == 0x01){
				if(PINA == 0x80){
					PORTB = 0x00;
				}
			}
			break;
		case CL_1st:
			break;
		case CL_2nd:
			break;
		case CL_3rd:
			break;
		case CL_4th:
			break;
		case CL_Flip:
			if(PORTB == 0x00){
				PORTB = 0x01;
			}
			else if(PORTB == 0x01){
				PORTB = 0x00;
			}
			break;
		default:
			break;	
	}
}

int main(void)
{
	/* Replace with your application code */
	while (1)
	{
		CL_Tick();
	}
}
