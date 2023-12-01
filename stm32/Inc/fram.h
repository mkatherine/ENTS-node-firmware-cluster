/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file     fram.h
  * @author   Stephen Taylor
  * @brief    This file contains all the function prototypes for
  *           the fram.c file
  * @date     11/17/2023
  ******************************************************************************
  */
/* USER CODE END Header */
#include "i2c.h"
#include "usart.h"
#include "fifo.h"

#include <stdio.h>

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
}configuration;

HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t len);
HAL_StatusTypeDef FRAM_Read(uint8_t *data);
HAL_StatusTypeDef configure_Settings(configuration c);
configuration read_Settings(void);
