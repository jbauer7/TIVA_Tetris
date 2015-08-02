#include "input.h"

volatile int down_pressed_cnt, down_released_cnt, up_pressed_cnt, left_pressed_cnt, right_pressed_cnt;
extern volatile uint8_t left_pressed, right_pressed, up_pressed, down_pressed, left_x, right_x, up_y, down_y;;

void handle_push_buttons(void) 
{
	 	// Examine push buttons at 400 hz, debouncing them so they only react if 
		// held for 16 ms
		if((GPIOF->DATA & DIR_BTN_DOWN) == DIR_BTN_DOWN) {
			down_pressed_cnt = 0;
			if((down_released_cnt++) == 7) {
				down_pressed = 0;
			}
		} else if((down_pressed_cnt++) == 7) {
			down_released_cnt = 0;
			down_pressed = 1;
		}
		
		// Same thing for UP push button
		if((GPIOF->DATA & DIR_BTN_UP) == DIR_BTN_UP) {
			up_pressed_cnt = 0;
		} else if((up_pressed_cnt++) == 7) {
			up_pressed = 1;
		}

		// Same thing for LEFT push button
		if((GPIOF->DATA & DIR_BTN_LEFT) == DIR_BTN_LEFT) {
			left_pressed_cnt = 0;
		} else if((left_pressed_cnt++) == 7) {
			left_pressed = 1;
		}

		// Same thing for RIGHT push button
		if((GPIOF->DATA & DIR_BTN_RIGHT) == DIR_BTN_RIGHT) {
			right_pressed_cnt = 0;
		} else if((right_pressed_cnt++) == 7) {
			right_pressed = 1;
		}		
}

void handle_joystick_X(void)
{
	uint32_t adc0_value;
	adc0_value = (ADC0->SSFIFO3 & 0xFFF);
	
	// Depending on the ADC value change the TAILR register on TIMER1
	// to adjust how often we rotate the LED_STRING array
	if(adc0_value <= 1365) {
			right_x = 1;
	}
	else{
			right_x = 0;
	}
	if(adc0_value >= 3185) {
			left_x = 1;
	}
	else{
			left_x = 0;
	}
}

void handle_joystick_Y(void)
{
	uint32_t adc0_value;
	adc0_value = (ADC0->SSFIFO2 & 0xFFF);
	
	// Depending on the ADC value change the TAILR register on TIMER1
	// to adjust how often we rotate the LED_STRING array
	if(adc0_value <= 1365) {
			down_y = 1;
	} 
	else{
			down_y = 0;
	}
		if(adc0_value >= 3185) {
			up_y = 1;
	}
	else{
			up_y =0;
	}
}
