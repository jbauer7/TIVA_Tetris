#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game_constants.h"
#include "led.h"
#include "eeprom.h"
#include "render.h"
#include "wireless.h"

void update(void);
void new_game(void);
void end_game(void);
void main_menu(void);

#endif
