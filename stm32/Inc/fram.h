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

HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t len);
HAL_StatusTypeDef FRAM_Read(uint8_t *data);
//HAL_StatusTypeDef configure_Settings(uint64_t cell_ID, uint64_t logger_ID, uint64_t gateway_EUI, uint64_t application_EUI, uint64_t end_device_EUI);