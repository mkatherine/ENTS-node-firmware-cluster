/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file     fifo.h
  * @author   Stephen Taylor
  * @brief    This file contains all the function prototypes for
  *           the fifo.c file
  * @date     11/17/2023
  ******************************************************************************
  */

#ifndef __FIFO_H__
#define __FIFO_H__

#ifdef __cplusplus
extern "C"{
#endif
/* USER CODE END Header */

/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "usart.h"

#include <stdio.h>

/* USER CODE END Includes */
/* USER CODE BEGIN PD */
#define FM24_WRITE 0xA0 // Device address of FM24 in write mode
#define FM24_READ 0xA1 // Device address of FM24 in read mode
#define MAXIMUM_MEMORY_ADDRESS 255
#define MAXIMUM_PAGE_ADDRESS 6
#define MINIMUM_PAGE_ADDRES 0


/* USER CODE END PD */

/* USER CODE BEGIN 1 */

/**
  ******************************************************************************
  * @brief    This function writes a dynamic number of bytes to FRAM.
  * 
  *           This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function uses global variables
  *           to keep track of what bytes have been written to, and than uses the HAL library
  *           to write to the FM24CL16B. 
  *           
  *           The FM24CL16B memory archtecture is structed around 8 segments of 
  *           256 bytes each. This function fills all 256 bytes in one segment before moving on to the next,
  *           it eventually loops back around and starts overwriting at segment 1, byte 1. Leaving the 8th segment for user
  *           configurable settings.
  * @param    data An array of data bytes.
  * @param    num_bytes The number of bytes to be written.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef push(const uint8_t *data, uint8_t num_bytes);

/**
  ******************************************************************************
  * @brief    This function reads a dynamic number of bytes to FRAM.
  * 
  *           This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function uses global variables
  *           to keep track of what bytes have been read from, and than uses the HAL library
  *           to read from the FM24CL16B.
  * @param    data Array to be read into
  * @param    num_bytes The number of bytes to be read.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef pop(uint8_t *data,  uint8_t num_bytes);

#ifdef __cplusplus
}
#endif

#endif /* __FIFO_H__ */

/* USER CODE END 1 */