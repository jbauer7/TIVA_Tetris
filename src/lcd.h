#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

#include "../include/eadogs102w.h"

#define SCREEN_HEIGHT_PG    8
#define SCREEN_HEIGHT_PX   64
#define SCREEN_WIDTH_PX   102
#define PAGE_HEIGHT_PX      8

#define COLUMN_CLEAR        0

void init_lcd_gpio(void);
void draw_buffer(void);
void draw_buffer_game_board(void);
void clear_buffer(void);
void fill_pixel(int x, int y);
void clear_buffer_game_board(void);


#endif
