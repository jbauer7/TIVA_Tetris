#ifndef __EEPROM_H__
#define __EEPROM_H__


#include <stdint.h>
#include "i2c.h"

i2c_status_t eeprom_byte_write
( 
  uint32_t  i2c_base,
  uint16_t  eepromDataAddress,
  uint8_t   data
);

i2c_status_t eeprom_byte_read
( 
  uint32_t  i2c_base,
  uint16_t  address,
  uint8_t   *data
);

void set_high_score(uint32_t high_score);
void get_high_score(uint32_t *high_score);

#endif
