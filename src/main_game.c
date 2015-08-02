#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "game_constants.h"
#include "lcd.h"
#include "render.h"
#include "game_logic.h"


extern volatile uint8_t left_pressed;
extern volatile uint8_t right_pressed;
extern volatile uint8_t up_pressed;
extern volatile uint8_t down_pressed;

void
teardown_screen(void)
{
  endwin();
}

void
extra_init(void)
{
  keypad(stdscr, TRUE);
  halfdelay(1);
}

void
get_direction(void)
{
  int ch = getch();

  switch(ch) {
  case KEY_LEFT:
    left_pressed = 1;
    break;
  case KEY_RIGHT:
    right_pressed = 1;
    break;
  case KEY_UP:
    up_pressed = 1;
    break;
  case KEY_DOWN:
    down_pressed = 1;
    break;
  default:
    break;
  }
}

int
main(int argc, char** argv)
{
  init_lcd_gpio();
  extra_init();
  new_game();

  while(1) {
    get_direction();
    update();
    print_screen();
    //get_direction();

    usleep(DELAY);
  }

  teardown_screen();

  return 0;
}
