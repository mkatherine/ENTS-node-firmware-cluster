/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file     fram.h
  * @author Stephen Taylor
  * @brief    This file contains all the function prototypes for
  *           the fram.c file
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
#define MAXIMUM_PAGE_ADDRESS 7
#define MINIMUM_PAGE_ADDRES 0

/* USER CODE END PD */

/* USER CODE BEGIN 1 */
extern int page_memory; // Global tracker of the page memory
extern int address_memory; // Global tracker of the address memory

HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t num_bytes);
HAL_StatusTypeDef FRAM_Read(uint8_t *data,  uint8_t num_bytes);


/* USER CODE END 1 */