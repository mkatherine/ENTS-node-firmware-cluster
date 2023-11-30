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
#define USER_DATA_PAGE_ADDRESS 0x07
#define CELL_ID_MEMORY_ADDRESS 0x00
#define LOGGER_ID_MEMORY_ADDRESS 0x08
#define LORA_GATEWAY_EUI_MEMORY_ADDRESS 0x10
#define LORA_APPLICATION_EUI_MEMORY_ADDRESS 0x17
#define LORA_END_DEVICE_EUI_MEMORY_ADDRESS 0x1f

/* USER CODE END PD */

/* USER CODE BEGIN 1 */

HAL_StatusTypeDef push(const uint8_t *data, uint8_t num_bytes);
HAL_StatusTypeDef pop(uint8_t *data,  uint8_t num_bytes);
HAL_StatusTypeDef configure_Settings(uint64_t cell_ID, uint64_t logger_ID, uint64_t gateway_EUI, uint64_t application_EUI, uint64_t end_device_EUI);



/* USER CODE END 1 */