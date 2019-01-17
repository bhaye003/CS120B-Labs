/**
 * Copyright (c) 2017 Microchip Technology Inc.
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "sam.h"

/* TZ_START_NS: Start address of non-secure application */
#define TZ_START_NS 0x00008000

/* USER_WORD_X: User Row (UROW) Word X definitions */
#define USER_WORD_0 0xB08F437F	/* BOD, Watchdog and Misc settings */
#define USER_WORD_1 0xFFFFF8BB	/* Watchdog and Misc settings */
#define USER_WORD_2 0x40082080	/* Memories Security Attribution: AS = 0x80, ANSC = 0x20, RS = 0x40 */
#define USER_WORD_3 0xFFFFFFFF	/* User Row Write Enable */
#define USER_WORD_4 0x00000000	/* Peripherals Security Attribution Bridge A (NONSECA) */
#define USER_WORD_5 0x00000000	/* Peripherals Security Attribution Bridge B (NONSECB) */
#define USER_WORD_6 0x00000000	/* Peripherals Security Attribution Bridge C (NONSECC) */

__attribute__((section (".userrowsec"))) 
const unsigned long userRow[7] = { USER_WORD_0 , USER_WORD_1 , USER_WORD_2, USER_WORD_3, USER_WORD_4, USER_WORD_5, USER_WORD_6 };
 	 	
/* typedef for non-secure callback functions */
typedef void (*funcptr_void) (void) __attribute__((cmse_nonsecure_call));
	
/*
	The Secure Application:
	* Sets the non-secure main stack (MSP_NS) 
	* Gets the non-secure reset handler
	* Jumps to the non-secure software application
*/

/* Secure main() */
int main(void)
{
	funcptr_void NonSecure_ResetHandler;
  	
	/* Initialize the SAM system */
    SystemInit();

	/* Set non-secure main stack (MSP_NS) */
	__TZ_set_MSP_NS(*((uint32_t *)(TZ_START_NS)));
  
	/* Get non-secure reset handler */
	NonSecure_ResetHandler = (funcptr_void)(*((uint32_t *)((TZ_START_NS) + 4U)));
  
	/* Start non-secure state software application */
	NonSecure_ResetHandler();
  
    /* Replace with your SECURE application code */
    while (1) 
    {
		__NOP();
    }
}
