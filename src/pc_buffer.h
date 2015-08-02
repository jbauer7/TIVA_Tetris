#ifndef __PC_BUFFER_H__
#define __PC_BUFFER_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    volatile uint32_t produce_count;
    volatile uint32_t consume_count;
    uint16_t BUFFER_SIZE;
    volatile char *array;
} PC_Buffer ;

//*****************************************************************************
// Initializes the producer-consumer buffer structure.  Both the producer
// and consumer counts should be set to 0.
//*****************************************************************************
void pc_buffer_init(PC_Buffer *buffer, char* array, uint16_t buffer_size);

//*****************************************************************************
// Adds a character to the producer consumer buffer
//
// Add the new data to array[produce_count].  Increments produce_count
//*****************************************************************************
void pc_buffer_add(PC_Buffer *buffer, char data);

//*****************************************************************************
// Removes a character from the producer consumer buffer.  Returned in data
//
// Returns the data at array[consume_count].  Increments  consume_count
//*****************************************************************************
void pc_buffer_remove(PC_Buffer *buffer, char *data);

//*****************************************************************************
// Returns if the producer consumer buffer is empty
//*****************************************************************************
bool pc_buffer_empty(PC_Buffer *buffer);

//*****************************************************************************
// Returns if the producer consumer buffer is full
//*****************************************************************************
bool pc_buffer_full(PC_Buffer *buffer);

#endif
