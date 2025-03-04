/**
 * @file fram.h
 * @author John Madden (jmadden173@pm.me)
 * @brief
 * @version 0.1
 * @date 2024-10-02
 *
 * Function calls for read/write ensures data does not exceed size of the
 * buffers.
 *
 * TODO:
 * - Add function for checking the device ID as a test. Export expected output
 * in each library
 * - Fix multiple pages test to be universal across boards
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LIB_STORAGE_INCLUDE_FRAM_H_
#define LIB_STORAGE_INCLUDE_FRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "stm32wlxx_hal.h"
#include "sys_app.h"

#define USER_DATA_PAGE_ADDRESS 0x07
#define CELL_ID_MEMORY_ADDRESS 0x00
#define LOGGER_ID_MEMORY_ADDRESS 0x08
#define LORA_GATEWAY_EUI_MEMORY_ADDRESS 0x10
#define LORA_APPLICATION_EUI_MEMORY_ADDRESS 0x17
#define LORA_END_DEVICE_EUI_MEMORY_ADDRESS 0x1f
#define LOGGING_INTERVAL_IN_SECONDS_MEMORY_ADDRESS 0x27
#define UPLOAD_INTERVAL_IN_MINUTES_MEMORY_ADDRESS 0x29

#define DUMP_FRAM_DISPLAY_HEX 0
#define DUMP_FRAM_DISPLAY_DECIMAL 1
#define DUMP_FRAM_OMIT_NONE 0
#define DUMP_FRAM_OMIT_JUNK 1

typedef struct user_configurations {
  uint64_t cell_ID;
  uint64_t logger_ID;
  uint64_t gateway_EUI;
  uint64_t application_EUI;
  uint64_t end_device_EUI;
  uint16_t logging_interval;
  uint16_t upload_interval;
} configuration;

/** Status codes for the Fram library*/
typedef enum {
  FRAM_OK = 0,
  FRAM_ERROR = -1,
  FRAM_OUT_OF_RANGE = -2,
  FRAM_BUFFER_FULL = -3,
  FRAM_BUFFER_EMPTY = -4,
} FramStatus;

/** Address size definition */
typedef uint32_t FramAddr;

/**
 * @brief Writes bytes to an address
 *
 * @param addr Address of write
 * @param data An array of data bytes.
 * @param len The number of bytes to be written.
 * @return See FramStatus
 */
FramStatus FramWrite(FramAddr addr, const uint8_t *data, size_t len);

/**
 * @brief    This function reads a dynamic number of bytes to FRAM.
 *
 * @param addr Address of read
 * @param data Array to be read into
 * @param len Number of sequential bytes to read
 * @return See FramStatus
 */
FramStatus FramRead(FramAddr addr, size_t len, uint8_t *data);

/**
 * @brief Get the number of available bytes in FRAM
 *
 * @return Number of bytes
 */
FramAddr FramSize(void);

/**
 * @brief Get number of pages
 *
 * A page is defined as a space of memory requiring the change of address.
 *
 * @return Number of pages
 */
unsigned int FramPages(void);

/**
 * @brief Gets the size of a segment
 *
 * @return Number of bytes in each segment
 */
unsigned int FramSegmentSize(void);

/**
 * @brief This function reads the user configurable settings from
 * non-volatile memory.
 *
 * @return configuration, an instance of the typedef struct
 * user_configurations.  Containing all the user defined settings to be
 * stored in non-volatile memory.
 */
configuration ReadSettings(void);

//
/**
 * @brief This function reads the entirety of non-volatile memory and
 * prints it. Only call FramDump() with input arg linesize=16.
 *
 * @param linesize Number of columns to display. Powers of 2 are recommended.
 * @param displayformat DUMP_FRAM_DISPLAY_HEX or DUMP_FRAM_DISPLAY_DECIMAL
 * @param omitjunk DUMP_FRAM_OMIT_NONE to print all memory in the specified
 * range or DUMP_FRAM_OMIT_JUNK to omit lines that do not hold useful data
 * @param printdelay_ms Delay between printing lines in milliseconds (ms).
 * Recommended value 50 ms.
 * @param startaddress Memory address to start the dump at (inclusive). The
 * linesize aligned memory address will be used.
 * @param endaddress Memory address to end the dump at (inclusive).
 * @return see FramStatus
 */
FramStatus FramDump(uint16_t linesize, uint8_t displayformat, uint8_t omitjunk,
                    uint8_t printdelay_ms, uint16_t startaddress,
                    uint16_t endaddress);

#ifdef __cplusplus
}
#endif

#endif  // LIB_STORAGE_INCLUDE_FRAM_H_
