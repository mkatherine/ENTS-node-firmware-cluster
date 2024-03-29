/**
 * @file     fifo.h
 * @author   Stephen Taylor
 * @brief    Circular buffer as FIFO data structure for buffering measurements
 * 
 * A circular buffer is implemented on part of the memory space of the fram
 * chip. The address space used can be modified with FRAM_BUFFER_START and
 * FRAM_BUFFER_END depending on user needs.
 * 
 * Measurements are stored with a single uint8_t of their length followed by
 * the serialized protobuf message. On reads, the length is first read, then
 * the length number of bytes are read into RAM. Ensure the read buffer used
 * is of sufficient size.
 * 
 * The buffer's implementation does not allow for overwriting of data. Once
 * the buffer is full, indicated by FRAM_BUFFER_FULL, data needs to be removed
 * by getting the next measurement or clearing the buffer entirely.
 * 
 * @date     11/17/2023
 */

#ifndef __FRAM_QUEUE_H__
#define __FRAM_QUEUE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

#include "i2c.h"

#include "fram.h"


#ifndef FRAM_BUFFER_START
/** Starting address of buffer */
#define FRAM_BUFFER_START 0
#endif /* FRAM_BUFFER_START */


#ifndef FRAM_BUFFER_END
/** Ending address of buffer */
#define FRAM_BUFFER_END 1792
#endif /* FRAM_BUFFER_END */

#if FRAM_BUFFER_START > FRAM_BUFFER_END
#error "Buffer end address must be greater than buffer start address"
#endif


/**
 * @brief Puts a measurement into the circular buffer
 * 
 * @param    data An array of data bytes.
 * @param    num_bytes The number of bytes to be written.
 * @return   See FramStatus
 */
FramStatus FramPut(const uint8_t *data, uint16_t num_bytes);

/**
 * @brief    Reads a measurement from the queue
 * 

 * @param    data Array to be read into
 * @return   See FramStatus
 */
FramStatus FramGet(uint8_t *data);

/**
 * @brief Get the current number of measurements stored in the buffer
 * 
 * @return Number of measurements
 */
uint16_t FramBufferLen(void);

#ifdef __cplusplus
}
#endif

#endif /* __FRAM_QUEUE_H__ */