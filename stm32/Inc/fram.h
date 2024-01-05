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

#ifndef __FRAM_H__
#define __FRAM_H__

#ifdef __cplusplus
extern "C"{
#endif
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

/**
  ******************************************************************************
  * @brief    This function writes a dynamic number of bytes to FRAM.
  * 
  *           This function is a wrapper for the fifo library. This library is used
  *           to write serialized logger data to the onboard FM24CL16B. 
  * @param    data An array of data bytes.
  * @param    len The number of bytes to be written.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t len);

/**
  ******************************************************************************
  * @brief    This function reads a dynamic number of bytes to FRAM.
  * 
  * @param    data Array to be read into
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef FRAM_Read(uint8_t *data);

/**
  ******************************************************************************
  * @brief    This function stores user configurable settings to non-volatile memory.
  *           Specifically cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application EUI and 
  *           end device EUI. As well as the logging and upload intervals.
  * 
  * @param    configuration, an instance of the typdef struct user_configurations. Containing all the user defined settings to be stored in non-volatile memory.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef configure_Settings(configuration c);

/**
  ******************************************************************************
  * @brief    This function reads the user configurable settings from non-volatile memory.
  * 
  * @param    void, an instance of the typdef struct user_configurations. Containing all the user defined settings to be stored in non-volatile memory.
  * @return   configuration, an instance of the typdef struct user_configurations. Containing all the user defined settings to be stored in non-volatile memory.
  ******************************************************************************
  */
configuration read_Settings(void);

#ifdef __cplusplus
}
#endif

#endif /* __FRAM_H__ */