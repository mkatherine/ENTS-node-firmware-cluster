/**
  ******************************************************************************
  * @file    fram.c
  * @author  Stephen Taylor
  * @brief   Soil Power Sensor FRAM library
  *          This file provides functions to read/write to the onboard FM24CL16B 
  *          FRAM memory device. The FM24CL16B is used to store serialized measurment 
  *          data before it is uploaded via LoRa or WiFi.
  *          
  *          The FM24CL16B memory archtecture is structed around 8 segments of 
  *          256 bytes each. The measurment data is stored on the first 7 segments in
  *          as a FIFO data structure. The 8th page is reserved for user configurable settings
  *          with space left over for whatever information the user wishes to store in 
  *          non-volatile memory. 
  * @date    11/17/2023
  *
  ******************************************************************************
  **/

 /* Includes ------------------------------------------------------------------*/
#include "fram.h"

/**
  ******************************************************************************
  * @brief    This function writes a dynamic number of bytes to FRAM.
  * 
  *           This function is a wrapper for the fifo library. This library is used
  *           to write serialized logger data to the onboard FM24CL16B. 
  *           
  *           The FM24CL16B memory archtecture is structed around 8 segments of 
  *           256 bytes each. This function fills all 256 bytes in one segment before moving on to the next,
  *           it eventually loops back around and starts overwriting at segment 1, byte 1. Leaving the 8th segment for user
  *           configurable settings.
  * @param    data An array of data bytes.
  * @param    len The number of bytes to be written.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t len){
    HAL_StatusTypeDef status;
    // uint8_t len;
    // char output[11]; // Having a lot of issues calcualting len in function
    // len = sizeof(&data); // calculate length of data in bytes
    status = push(&len, 1); // Push the number of bytes in the serialized data block
    if (status != HAL_OK){ // Return if I2C communication fails
        return status;
    }
    status = push(data, len); // push the serialized data
    return status; // return I2C status
}

/**
  ******************************************************************************
  * @brief    This function reads a dynamic number of bytes to FRAM.
  * 
  *           This function is a wrapper for the fifo library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function uses global variables
  *           to keep track of what bytes have been read from, and than uses the HAL library
  *           to read from the FM24CL16B.
  * @param    data Array to be read into
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef FRAM_Read(uint8_t *data){
    HAL_StatusTypeDef status;
    uint8_t len[1];
    
    status = pop(len, 1); // pop the number of bytes to be read 
    
    if (status != HAL_OK){ // Return if I2C communication fails
        return status;
    }
    status = pop(data, len[0]); // pop the serialized data block
    return status;

}

/**
  ******************************************************************************
  * @brief    This function stores user configurable settings to non-volatile memory.
  *           Specifically cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application EUI and 
  *           end device EUI. As well as the logging and upload intervals.
  * 
  *           This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function stores the cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application EUI,
  *           end device EUI, logging interval and upload interval at the hard-coded memory adresses: 0x00, 0x08, 0x10, 0x17, 0x1f, 0x27 and 0x29 respectivley. This data is stored on the FM24CL16B's 
  *           8th page of memory.
  *           
  * @param    configuration, an instance of the typdef struct user_configurations. Containing all the user defined settings to be stored in non-volatile memory.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
 HAL_StatusTypeDef configure_Settings(configuration c){
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), CELL_ID_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.cell_ID, 8, 10); // Store the cell ID
    if (status != HAL_OK){ 
        return status; 
    }
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LOGGER_ID_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.logger_ID, 8, 10); // Store the logger ID
    if (status != HAL_OK){ 
        return status; 
    }
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LORA_GATEWAY_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.gateway_EUI, 8, 10); // Store the gateway eui
    if (status != HAL_OK){ 
        return status; 
    }
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LORA_APPLICATION_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.application_EUI, 8, 10); // Store the application eui
    if (status != HAL_OK){ 
        return status; 
    }
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LORA_END_DEVICE_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.end_device_EUI, 8, 10); // Store the application eui
    if (status != HAL_OK){ 
        return status; 
    }
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LOGGING_INTERVAL_IN_SECONDS_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.logging_interval, 2, 10); // Store the logging interval
    if (status != HAL_OK){ 
        return status; 
    }
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), UPLOAD_INTERVAL_IN_MINUTES_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.upload_interval, 2, 10); // Store the upload interval
    if (status != HAL_OK){ 
        return status; 
    }
    return status;
  }

/**
  ******************************************************************************
  * @brief    This function reads the user configurable settings from non-volatile memory.
  * 
  *           
  * @param    void, an instance of the typdef struct user_configurations. Containing all the user defined settings to be stored in non-volatile memory.
  * @return   configuration, an instance of the typdef struct user_configurations. Containing all the user defined settings to be stored in non-volatile memory.
  ******************************************************************************
  */
 configuration read_Settings(void){
    configuration c;
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), CELL_ID_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.cell_ID, 8, 10);
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), LOGGER_ID_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.logger_ID, 8, 10);
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), LORA_GATEWAY_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.gateway_EUI, 8, 10);
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), LORA_APPLICATION_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.application_EUI, 8, 10);
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), LORA_END_DEVICE_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.end_device_EUI, 8, 10);
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), LOGGING_INTERVAL_IN_SECONDS_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.logging_interval, 2, 10);
    HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), UPLOAD_INTERVAL_IN_MINUTES_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.upload_interval, 2, 10);
    return c;
 }