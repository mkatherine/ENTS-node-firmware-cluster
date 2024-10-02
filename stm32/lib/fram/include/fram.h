/**
 * @file fram.h
 * @author John Madden (jmadden173@pm.me)
 * @brief
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LIB_FRAM_INCLUDE_FRAM_H_
#define LIB_FRAM_INCLUDE_FRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "stm32wlxx_hal_def.h"

#define USER_DATA_PAGE_ADDRESS 0x07
#define CELL_ID_MEMORY_ADDRESS 0x00
#define LOGGER_ID_MEMORY_ADDRESS 0x08
#define LORA_GATEWAY_EUI_MEMORY_ADDRESS 0x10
#define LORA_APPLICATION_EUI_MEMORY_ADDRESS 0x17
#define LORA_END_DEVICE_EUI_MEMORY_ADDRESS 0x1f
#define LOGGING_INTERVAL_IN_SECONDS_MEMORY_ADDRESS 0x27
#define UPLOAD_INTERVAL_IN_MINUTES_MEMORY_ADDRESS 0x29

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

/**
 * @brief This function stores user configurable settings to non-volatile
 * memory.
 *
 * Specifically cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application
 * EUI and end device EUI. As well as the logging and upload intervals.
 *
 * @param configuration an instance of the typedef struct user_configurations.
 * Containing all the user defined settings to be stored in non-volatile
 * memory.
 *
 * @return HAL_StatusTypeDef, status of the I2C function
 */
HAL_StatusTypeDef ConfigureSettings(configuration c);

/**
 * @brief This function reads the user configurable settings from non-volatile
 * memory.
 *
 * @return configuration, an instance of the typedef struct
 * user_configurations.  Containing all the user defined settings to be stored
 * in non-volatile memory.
 */
configuration ReadSettings(void);

#ifdef __cplusplus
}
#endif

#endif  // LIB_FRAM_INCLUDE_FRAM_H_
