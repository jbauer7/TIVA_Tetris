#include "game_logic.h"

volatile uint8_t left_pressed, 
  right_pressed,
  up_pressed,
  down_pressed,
	left_x, 
  right_x,
  up_y,
  down_y,
  current_x,
  current_y,
  current_tetrimino,
  next_tetrimino,
	game_state, 
  current_rotation,
  move_down,
	menu_selection,
	old_menu_selection,
	time_over;

volatile bool DropFlag;
volatile uint8_t game_board[BOARD_WIDTH][BOARD_HEIGHT];
volatile int score, op_score;
extern volatile uint32_t high_score;

extern volatile int done_count;
extern uint8_t p1_wireless_addr[5];
extern uint8_t p2_wireless_addr[5];

uint8_t open_spot(int x, int y)
{
  if(x < 0 || x >= BOARD_WIDTH || y <= 0 || game_board[x][y])
    return 0;

  return 1;
}

uint8_t valid_move(int x_delta, int y_delta, int rotation)
{
  int i, j, x, y;
  Tetrimino *tetrimino = &tetriminos[current_tetrimino];

  x = current_x + x_delta;
  y = current_y + y_delta;

  for(i = 0; i < TETRIMINO_BMP_W; i++) {
    for(j = 0; j < TETRIMINO_BMP_H; j++) {
      if(tetrimino->bitmap[j * TETRIMINO_BMP_W + i] == 1) {
	switch(rotation) {
	case ROTATE_0_DEG:
	  if(!open_spot((x - tetrimino->x_center) + i, y - j))
	    return 0;
	  break;
	case ROTATE_90_DEG:
	  if(!open_spot(x - j, (y + tetrimino->x_center) - i))
	    return 0;
	  break;
	case ROTATE_180_DEG:
	  if(!open_spot((x + tetrimino->x_center) - i, y + j))
	    return 0;
	  break;
	case ROTATE_270_DEG:
	  if(!open_spot(x + j, (y - tetrimino->x_center) + i))
	    return 0;
	  break;
	}
      }
    }
  }

  return 1;
}

void new_piece(void)
{
  current_x = STARTING_X;
  current_y = STARTING_Y;
  current_rotation = ROTATE_0_DEG;
  current_tetrimino = next_tetrimino;
	next_tetrimino= rand() % NUM_TETRIMINOS;
	load_led_array(next_tetrimino);

  if(!valid_move(0,0,current_rotation)) {
		score = 0;
    end_game();
  }
	
	score += 5;
}

void remove_lines(void)
{
  int i, j, lines_cleared, line_full, resume_line;

  lines_cleared = 0;

  for(j = BOARD_HEIGHT; j >= 0; j--) {
    line_full = 1;
    for(i = 0; i < BOARD_WIDTH; i++) {
      if(game_board[i][j] == 0)
	line_full = 0;
    }

    if(line_full) {
      lines_cleared++;
      resume_line = j;

      for(j = j; j < BOARD_HEIGHT - 1; j++) {
	for(i = 0; i < BOARD_WIDTH; i++) {
	  game_board[i][j] = game_board[i][j+1];
	}
      }

      j = resume_line;
    }
  }

  switch(lines_cleared){
  case 1:
    score += 100;
    break;
  case 2:
    score += 300;
    break;
  case 3:
    score += 500;
    break;
  case 4:
    score += 800;
    break;
  }
}

void settle_piece(void)
{
  int i, j, x, y;
  Tetrimino *tetrimino = &tetriminos[current_tetrimino];

  x = current_x;
  y = current_y;

  for(i = 0; i < TETRIMINO_BMP_W; i++) {
    for(j = 0; j < TETRIMINO_BMP_H; j++) {
			if(tetrimino->bitmap[j * TETRIMINO_BMP_W + i] == 1) {
	switch(current_rotation) {
	case ROTATE_0_DEG:
	  game_board[(x - tetrimino->x_center) + i][y - j] = 1;
	  break;
	case ROTATE_90_DEG:
	  game_board[x - j][(y + tetrimino->x_center) - i] = 1;
	  break;
	case ROTATE_180_DEG:
	  game_board[(x + tetrimino->x_center) - i][y + j] = 1;
	  break;
	case ROTATE_270_DEG:
	  game_board[x + j][(y - tetrimino->x_center) + i] = 1;
	  break;
	}
      }
    }
  }

  remove_lines();
  new_piece();
}

void update(void)
{
	if(game_state == STATE_GAME) {
		if((left_pressed|| left_x)&& valid_move(-1, 0, current_rotation)) {
			current_x--;
		} else if((right_pressed|| right_x)&& valid_move(1, 0, current_rotation)) {
			current_x++;
		} else if((up_pressed || up_y) && (valid_move(0, 0, ((current_rotation + 1) % 4)) || (current_tetrimino == TETRIMINO_I))) {
			if(current_tetrimino == TETRIMINO_O) {
				up_pressed = 0;
				return;
			}

			if(current_tetrimino == TETRIMINO_I) {
				if(current_rotation == ROTATE_0_DEG && valid_move(0, 0, ROTATE_90_DEG))
		current_rotation = ROTATE_90_DEG;
				else if(valid_move(0, 0, ROTATE_0_DEG))
		current_rotation = ROTATE_0_DEG;

				up_pressed = 0;
				return;
			}

			current_rotation = ((current_rotation + 1) % 4);
		}

		if(DropFlag && valid_move(0, -1, current_rotation)){
			current_y--;
			DropFlag = 0;
		}else if(DropFlag) {
			settle_piece();
			DropFlag = 0;
		}

		left_pressed = 0;
		right_pressed = 0;
		up_pressed = 0;
	} else if (game_state == STATE_MENU) {
		
		if(left_pressed) {
			menu_selection = 0;
		} else if(right_pressed) {
			menu_selection = 1;
		} else if (up_pressed) {
			DisableInterrupts();
			if(menu_selection) {
				wireless_configure_device(p2_wireless_addr, p1_wireless_addr);
			} else {
				wireless_configure_device(p1_wireless_addr, p2_wireless_addr);
			}
			EnableInterrupts();
			
			new_game();
		}
		
		left_pressed = 0;
		right_pressed = 0;
		up_pressed = 0;
	} else {
		left_pressed = 0;
		right_pressed = 0;
		
		if(up_pressed) {
			up_pressed = 0;	
			
			if(time_over == 1) {
				main_menu();
			}
		}
	}
}

void end_game()
{
	if(game_state == STATE_GAME) {
		if(score > high_score) {
			high_score = score;
			set_high_score(score);
		}
		
		if(op_score > high_score) {
			high_score = op_score;
			set_high_score(op_score);
		}
		
		game_state = STATE_SCOREBOARD;
	}
}

void main_menu(void)
{
	menu_selection = 0;
	old_menu_selection = 1;
	game_state = STATE_MENU;
}

void new_game()
{
  int i, j;

  right_pressed = 0;
  left_pressed = 0;
  up_pressed = 0;
  down_pressed = 0;
	right_x = 0;
  left_x = 0;
  up_y = 0;
  down_y = 0;
  score = 0;
	op_score = 0;
	done_count = 0;
	time_over = 0;
  current_x = STARTING_X;
  current_y = STARTING_Y;
  current_rotation = ROTATE_0_DEG;
  current_tetrimino = rand() % NUM_TETRIMINOS;
	next_tetrimino = rand() % NUM_TETRIMINOS;
	game_state = STATE_GAME;
	load_led_array(next_tetrimino);
	init_print_screen();
	WATCHDOG0->ICR = 1;
	
  for(i = 0; i < BOARD_WIDTH; i++) {
    for(j = 0; j < BOARD_HEIGHT; j++) {
      game_board[i][j] = 0;
    }
  }
}
