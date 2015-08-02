#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdint.h>

#include "../include/eadogs102w.h"
#include "lcd.h"
#include "game_constants.h"
#include "game_logic.h"

////////////////////////////////////////////////
///////    Rendering Related Constants   ///////
////////////////////////////////////////////////

#define SCREEN_WIDTH_PX    102
#define SCREEN_HEIGHT_PX    64
#define TILE_HEIGHT_PX       3
#define TILE_WIDTH_PX        3

#define BOARD_START_X        0
#define BOARD_START_Y        0

void draw_tile(int screen_x, int screen_y);
void draw_tetrimino(void);
void draw_board_border(void);
void draw_board_contents(void);
void init_print_screen(void); 
void loop_print_screen(void);

#endif
