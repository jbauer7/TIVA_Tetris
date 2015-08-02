#ifndef __IO_EXPANDER_H__
#define __IO_EXPANDER_H__


#include <stdint.h>
#include "i2c.h"
#include "boardUtil.h"


void init_io_expander(void);
void ledMatrixWriteData( uint32_t i2cBase, uint8_t colNum, uint8_t data);

#endif
