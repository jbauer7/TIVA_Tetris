#include "render.h"

extern volatile uint8_t current_x;
extern volatile uint8_t current_y;
extern volatile uint8_t current_rotation;
extern volatile uint8_t current_tetrimino;
extern volatile uint8_t game_state, menu_selection, old_menu_selection;
extern volatile uint8_t game_board[BOARD_WIDTH][BOARD_HEIGHT];
extern volatile int score, op_score, high_score;

volatile uint8_t score_drawn;

int old_score, old_op_score;

void
draw_tetrimino()
{
  uint8_t i, j, x, y;
  Tetrimino *tetrimino = &tetriminos[current_tetrimino];

  x = current_x + BOARD_START_X + 1;
  y = (BOARD_HEIGHT - current_y) + BOARD_START_Y;

  for(i = 0; i < TETRIMINO_BMP_W; i++) {
    for(j = 0; j < TETRIMINO_BMP_H; j++) {
      if(tetrimino->bitmap[j * TETRIMINO_BMP_W + i] == 1)
	switch(current_rotation) {
	case ROTATE_0_DEG:
	  draw_tile((x - tetrimino->x_center) + i, y + j);
	  break;
	case ROTATE_90_DEG:
	  draw_tile(x - j, (y - tetrimino->x_center) + i);
	  break;
	case ROTATE_180_DEG:
	  draw_tile((x + tetrimino->x_center) - i, y - j);
	  break;
	case ROTATE_270_DEG:
	  draw_tile(x + j, (y + tetrimino->x_center) - i);
	  break;  
	}
    } 
  }
}

void
draw_tile(int screen_x, int screen_y)
{ 
  int tile_start_x, tile_start_y, tile_x_offset = 0, tile_y_offset = 0;
  
  for(tile_start_x = screen_x * TILE_WIDTH_PX; tile_x_offset < TILE_WIDTH_PX; tile_x_offset++) {
		tile_y_offset = 0;
    for(tile_start_y = screen_y * TILE_HEIGHT_PX; tile_y_offset < TILE_HEIGHT_PX; tile_y_offset++) {
      fill_pixel((tile_start_x + tile_x_offset), (tile_start_y + tile_y_offset));
    }
  } 
}

void
draw_board_border(void)
{
  int x , y;

  for(y = 0; y < BOARD_HEIGHT; y++) {
    draw_tile(BOARD_START_X, (y + BOARD_START_Y));
    draw_tile((BOARD_START_X + BOARD_WIDTH + 1), (y + BOARD_START_Y));
  }

  for(x = 0; x < BOARD_WIDTH + 2; x++) {
    draw_tile((x + BOARD_START_X), (BOARD_START_Y + BOARD_HEIGHT));
  }
}

void
draw_board_contents(void)
{
	int x , y;
	
	for(x = 0; x < BOARD_WIDTH + 2; x++) {
    draw_tile((x + BOARD_START_X), (BOARD_START_Y + BOARD_HEIGHT));
  }
	
	for(x = 0; x < BOARD_WIDTH; x++){
		for(y = 0; y < BOARD_HEIGHT; y++) {
      if(game_board[x][y])
				draw_tile(x + BOARD_START_X + 1, BOARD_HEIGHT - y);
    }
  }
}

void
draw_menu(void)
{
	lcd_clear();
	
	lcd_write_char_10pts(0, 'T', 1);
	lcd_write_char_10pts(0, 'I', 2);
	lcd_write_char_10pts(0, 'V', 3);
	lcd_write_char_10pts(0, 'A', 4);
	lcd_write_char_10pts(1, 'T', 1);
	lcd_write_char_10pts(1, 'e', 2);
	lcd_write_char_10pts(1, 't', 3);
	lcd_write_char_10pts(1, 'r', 4);
	lcd_write_char_10pts(1, 'i', 5);
	lcd_write_char_10pts(1, 's', 6);
	
	lcd_write_char_10pts(2, 'P', 2);
	lcd_write_char_10pts(2, '1', 3);
	
	lcd_write_char_10pts(2, 'P', 7);
	lcd_write_char_10pts(2, '2', 8);
	
	if(menu_selection == 0)
		lcd_write_char_10pts(2, '-', 1);
	else
		lcd_write_char_10pts(2, '-', 6);
}

void draw_scoreboard(void)
{
	lcd_clear();
	
	lcd_write_char_10pts(0, 'Y', 1);
	lcd_write_char_10pts(0, 'o', 2);
	lcd_write_char_10pts(0, 'u', 3);
	
	if(score > op_score) {
		lcd_write_char_10pts(0, 'W', 5);
		lcd_write_char_10pts(0, 'i', 6);
		lcd_write_char_10pts(0, 'n', 7);
		lcd_write_char_10pts(0, '!', 8);
	} else {
		lcd_write_char_10pts(0, 'L', 5);
		lcd_write_char_10pts(0, 'o', 6);
		lcd_write_char_10pts(0, 's', 7);
		lcd_write_char_10pts(0, 'e', 8);
		lcd_write_char_10pts(0, '!', 9);
	}
	
	lcd_write_char_10pts(2, 'H', 1);
	lcd_write_char_10pts(2, 'i', 2);
	lcd_write_char_10pts(2, 'S', 3);
	lcd_write_char_10pts(2, 'c', 4);
	lcd_write_char_10pts(2, 'o', 5);
	lcd_write_char_10pts(2, 'r', 6);
	lcd_write_char_10pts(2, 'e', 7);
	lcd_write_char_10pts(2, ':', 8);
	
	lcd_write_char_8pts(6, '0' + ((high_score / 100000) % 10), 7);
	lcd_write_char_8pts(6, '0' + ((high_score / 10000) % 10), 8);
	lcd_write_char_8pts(6, '0' + ((high_score / 1000) % 10), 9);
	lcd_write_char_8pts(6, '0' + ((high_score / 100) % 10), 10);
	lcd_write_char_8pts(6, '0' + ((high_score / 10) % 10), 11);
	lcd_write_char_8pts(6, '0' + (high_score % 10), 12);
}

void
draw_score_text(void)
{
	lcd_write_char_10pts(0, 'S', 4);
	lcd_write_char_10pts(0, 'c', 5);
	lcd_write_char_10pts(0, 'o', 6);
	lcd_write_char_10pts(0, 'r', 7);
	lcd_write_char_10pts(0, 'e', 8);
	lcd_write_char_10pts(0, ':', 9);
	
	lcd_write_char_10pts(2, 'O', 4);
	lcd_write_char_10pts(2, 'P', 5);
	lcd_write_char_10pts(2, ' ', 6);
	lcd_write_char_10pts(2, 'S', 7);
	lcd_write_char_10pts(2, 'C', 8);
	lcd_write_char_10pts(2, ':', 9);
}

void
draw_score_values(void)
{
	lcd_write_char_8pts(2, '0' + ((score / 100000) % 10), 11);
	lcd_write_char_8pts(2, '0' + ((score / 10000) % 10), 12);
	lcd_write_char_8pts(2, '0' + ((score / 1000) % 10), 13);
	lcd_write_char_8pts(2, '0' + ((score / 100) % 10), 14);
	lcd_write_char_8pts(2, '0' + ((score / 10) % 10), 15);
	lcd_write_char_8pts(2, '0' + (score % 10), 16);
	
	lcd_write_char_8pts(6, '0' + ((op_score / 100000) % 10), 11);
	lcd_write_char_8pts(6, '0' + ((op_score / 10000) % 10), 12);
	lcd_write_char_8pts(6, '0' + ((op_score / 1000) % 10), 13);
	lcd_write_char_8pts(6, '0' + ((op_score / 100) % 10), 14);
	lcd_write_char_8pts(6, '0' + ((op_score / 10) % 10), 15);
	lcd_write_char_8pts(6, '0' + (op_score % 10), 16);
}

void
init_print_screen(void)
{
  clear_buffer();

  draw_board_border();
	draw_board_contents();
  draw_tetrimino();
	draw_score_values();

  draw_buffer();
	draw_score_text();
	draw_score_values();
}

void
loop_print_screen(void)
{
	if(game_state == STATE_GAME) {
		clear_buffer_game_board();
		
		draw_tetrimino();
		draw_board_contents();

		draw_buffer_game_board();
		
		if(old_score != score || old_op_score != op_score) 
			draw_score_values();
		
		old_score = score;
		old_op_score = op_score;
	} else if (game_state == STATE_MENU) {
		
		if(old_menu_selection != menu_selection) {
			clear_buffer();
			draw_menu();
			
			old_menu_selection = menu_selection;
			
			score_drawn = 0;
		}
	} else {
		if(score_drawn == 0) {
			draw_scoreboard();
		}
	}
}
