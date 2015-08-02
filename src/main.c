// Copyright (c) 2014, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../include/fonts.h"
#include "../include/eadogs102w.h"

#include "boardUtil.h"
#include "input.h"
#include "TM4C123.h"
#include "lcd.h"
#include "main_game.h"
#include "led.h"
#include "game_logic.h"
#include "render.h"
#include "eeprom.h"
#include "game_constants.h"


/******************************************************************************
 * Global Variables
 *****************************************************************************/
volatile bool AlertSysTick;
volatile bool AlertTimer0A;
volatile bool AlertADC0SS3;
volatile bool AlertADC0SS2;
volatile bool UpdateScreen;
volatile bool SendScoreFlag;
volatile bool RFFlag;
volatile bool GameDoneFlag;
volatile uint32_t high_score;
extern volatile bool UpdateLED;
extern volatile uint8_t game_state, time_over, score_drawn;

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{	
	uint32_t status, data;
	init_board();
	//load high score from the eprom
	get_high_score(&high_score);
	
	//prints main menu to the LCD screen
	main_menu();
	
	// Uncomment to clear high score
	//set_high_score(0);
	
  // Infinite Loop
  while(1){
		if(AlertADC0SS3) {
			handle_joystick_X();
			AlertADC0SS3 = 0;
		}
		
		if(AlertADC0SS2) {
			handle_joystick_Y();
			AlertADC0SS2 = 0;
		}
		
		if(AlertSysTick) {
			handle_push_buttons();
			AlertSysTick = 0;
		}
		
		if(SendScoreFlag) {
			if(game_state == STATE_GAME || game_state == STATE_SCOREBOARD) {
				rf_send_score();
			}
			SendScoreFlag = 0;
		}
		
		//prints wireless statistics
		if(AlertTimer0A) {
			if(game_state == STATE_MENU || game_state == STATE_SCOREBOARD) {
				WATCHDOG0->ICR = 1; // Pet watchdog on main menu
			}
			print_rf_stats();
			AlertTimer0A = 0;
		}
		
		//sends next peice to the led display
		if(UpdateLED) {
			UpdateLED = 0;
			if(game_state == STATE_GAME) {
				print_led();
			}
		}
		
		if(RFFlag) {
			handle_rf_interrupt();
			RFFlag = 0;
		}
		
		if(UpdateScreen) {
			update();
			loop_print_screen();
			UpdateScreen = 0;
		}
		
		if(GameDoneFlag) {
			end_game();
			GameDoneFlag = 0;
			time_over = 1;
			score_drawn = 0;
		}
  };
}
