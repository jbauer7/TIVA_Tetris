#include "led.h"

int current_column;	


//prints out the next piece on the led display
void print_led(void){
				current_column = ((current_column + 1) % 5);
				
				ledMatrixWriteData(I2C1_BASE, current_column, next_piece[current_column]);
}	

//updates the next piece
void load_led_array(uint8_t new_piece){
	next_piece = (uint8_t*) Led_LUT[new_piece];
}