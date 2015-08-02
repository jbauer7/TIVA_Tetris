#ifndef _GAME_CONSTANTS_H_
#define _GAME_CONSTANTS_H_

#include <stdint.h>

////////////////////////////////////////////////
////////    Board Related Constants   //////////
////////////////////////////////////////////////

#define STARTING_X     5
#define STARTING_Y    18
#define BOARD_WIDTH   10
#define BOARD_HEIGHT  20

////////////////////////////////////////////////
///////    Main Loop Related Constants   ///////
////////////////////////////////////////////////

#define GAME_LENGTH_SEC  60

#define STATE_MENU        0
#define STATE_GAME				1
#define STATE_SCOREBOARD  2

////////////////////////////////////////////////
///////         Tetrimino Bitmaps        ///////
////////////////////////////////////////////////

typedef struct {
  uint8_t x_center;
  uint8_t bitmap[8];
} Tetrimino;

#define NUM_TETRIMINOS  7

#define TETRIMINO_I     0
#define TETRIMINO_L     1
#define TETRIMINO_J     2
#define TETRIMINO_O     3
#define TETRIMINO_S     4
#define TETRIMINO_Z     5
#define TETRIMINO_T     6

#define TETRIMINO_BMP_H 2
#define TETRIMINO_BMP_W 4

#define ROTATE_0_DEG    0
#define ROTATE_90_DEG   1
#define ROTATE_180_DEG  2
#define ROTATE_270_DEG  3

static Tetrimino tetriminos[7] = {
  {1, {1, 1, 1, 1,  // I
	  0, 0, 0, 0}},
  {1, {1, 1, 1, 0,  // L
	  1, 0, 0, 0}},
  {1, {1, 1, 1, 0,  // J
	  0, 0, 1, 0}},
  {0, {1, 1, 0, 0,  // O
	  1, 1, 0, 0}},
  {1, {0, 1, 1, 0,  // S
	  1, 1, 0, 0}},
  {1, {1, 1, 0, 0,  // Z
	  0, 1, 1, 0}},
  {1, {1, 1, 1, 0,  // T
	  0, 1, 0, 0}}
};


#endif
