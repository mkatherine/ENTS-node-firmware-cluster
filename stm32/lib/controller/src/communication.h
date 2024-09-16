#ifndef STM32_LIB_CONTROLLER_SRC_COMMUNICATION_H_
#define STM32_LIB_CONTROLLER_SRC_COMMUNICATION_H_

#include <stdint.h>

#include "soil_power_sensor.pb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  /** State in buffer */
  uint8_t* data;
  /** Size of buffer */
  size_t size;
  /** Length of data in buffer */
  size_t len;
} Buffer;

/** @brief Buffer for ControllerTransmit */
static Buffer tx = {};

/** Buffer for ControllerReceive */
static Buffer rx = {};

/**
 * @brief Send bytes to esp32
 * 
 * @param timeout Timeout duration in ms
 */
void ControllerTransmit(unsigned int timeout);

/**
 * @brief Receive bytes from esp32
 * 
 * @param timeout Timeout duration in ms
 */
void ControllerReceive(unsigned int timeout);

/**
 * @brief Send message and receive response from esp32
 * 
 * @param timeout Timeout duration in ms
 */
void ControllerTransaction(unsigned int timeout);

#ifdef __cplusplus
}
#endif

#endif // STM32_LIB_CONTROLLER_SRC_COMMUNICATION_H_