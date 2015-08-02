#ifndef __led_H__
#define __led_H__

#include <stdint.h>
#include "led_lut.h"
#include "io_expander.h"


void print_led(void);

void load_led_array(uint8_t new_piece);

#endif
