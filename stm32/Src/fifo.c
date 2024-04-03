/**
 * @file    fifo.c
 * @author  Stephen Taylor
 * @date    11/17/2023
 * 
 * @see fifo.h
 **/

#include "fifo.h"

static uint16_t read_addr = FRAM_BUFFER_START;
static uint16_t write_addr = FRAM_BUFFER_START;

/** Number of measurements stored in buffer */
static uint16_t buffer_len = 0;

/**
 * @brief Updates circular buffer address based on number of bytes
 * 
 * @param addr 
 * @param num_bytes 
 */
static inline void update_addr(uint16_t *addr, const uint16_t num_bytes) {
  *addr = (*addr + num_bytes) % fram_buffer_size;
}

FramStatus FramPut(const uint8_t *data, const uint16_t num_bytes) {
  // check remaining space
  uint16_t remaining_space = fram_buffer_size - (write_addr - read_addr);
  if (num_bytes+1 > remaining_space) {
    return FRAM_BUFFER_FULL;
  }

  FramStatus status;

  // write single byte length to buffer
  status = FramWrite(write_addr, &num_bytes, 1);
  if (status != FRAM_OK) {
    return status;
  }
  update_addr(&write_addr, 1);

  // Write data to FRAM circular buffer
  status = FramWrite(write_addr, data, num_bytes);
  if (status != FRAM_OK) {
    return status;
  }
  update_addr(&write_addr, num_bytes);

  // increment buffer length
  ++buffer_len;

  return FRAM_OK;
}

FramStatus FramGet(uint8_t *data, uint8_t *len) {
  // Check if buffer is empty
  if (buffer_len == 0) {
    return FRAM_BUFFER_EMPTY;
  }

  FramStatus status;

  status = FramRead(read_addr, 1, len);
  if (status != FRAM_OK) {
    return status;
  }
  update_addr(&read_addr, 1);
  

  // Read data from FRAM circular buffer
  status = FramRead(read_addr, *len, data);
  if (status != FRAM_OK) {
    return status;
  }
  update_addr(&read_addr, *len);

  // Decrement buffer length
  --buffer_len;

  return FRAM_OK;
}

uint16_t FramBufferLen(void) {
  return buffer_len;
}

FramStatus FramBufferClear(void) {
  // Set read and write addresses to their default values
  read_addr = FRAM_BUFFER_START;
  write_addr = FRAM_BUFFER_START;

  // reset buffer len
  buffer_len = 0;

  return FRAM_OK;
}
