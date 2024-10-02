// Copyright 2023 UCSC

/**
 * @file     fram.h
 * @author   Stephen Taylor
 * @brief    This file contains all the function prototypes for the fram.c file
 *
 * The FM24CL16B memory architecture is structured around 8 segments of 256
 * bytes each. The library can be used with other memory chips with the same
 * memory layout.  The defines FRAM_PAGES and FRAM_SEG_SIZE can used to change
 * the configuration from the compiler.
 *
 * Internally addressing is converted from matrix-style (pages and segments) to
 * a flat memory address space starting at 0x0. The max available memory
 * address is stored in fram_max_addr that can be used for higher level
 * implementations.
 *
 * Currently user configuration template is provided by this library but will
 * moved in a future version.
 *
 * @date     11/17/2023
 */

#ifndef LIB_FRAM_SRC_FM24CL16B_H_
#define LIB_FRAM_SRC_FM24CL16B_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#include "fram.h"
#include "i2c.h"

#ifndef FRAM_PAGES
/** Number of pages on the fram chip */
#define FRAM_PAGES 8
#endif

#ifndef FRAM_SEG_SIZE
/** Size of each memory segment in bytes */
#define FRAM_SEG_SIZE 256
#endif

/** Max writeable address */
static const uint16_t fram_max_addr = (FRAM_SEG_SIZE * FRAM_PAGES) - 1;

/**
 * @brief Writes bytes to an address
 *
 * @param addr Address of write
 * @param data An array of data bytes.
 * @param len The number of bytes to be written.
 * @return See FramStatus
 */
FramStatus Fm24cl16bWrite(uint16_t addr, const uint8_t *data, uint8_t len);

/**
 * @brief    This function reads a dynamic number of bytes to FRAM.
 *
 * @param addr Address of read
 * @param data Array to be read into
 * @param len Number of sequential bytes to read
 * @return See FramStatus
 */
FramStatus Fm24cl16bRead(uint16_t addr, uint8_t len, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif  // LIB_FRAM_SRC_FM24CL16B_H_
