/**
  ******************************************************************************
  * @file    fram.c
  * @author  Stephen Taylor
  * @brief   Soil Power Sensor FRAM library
  *          This file provides functions to read/write to the onboard FM24CL16B 
  *          FRAM memory device. The FM24CL16B is used to store serialized measurment 
  *          data before it is uploaded via LoRa or WiFi.
  * @date    11/17/2023
  *
  ******************************************************************************
  **/

 /* Includes ------------------------------------------------------------------*/
#include "fram.h"

int address_memory = 0x00;
int page_memory = 0x00;


/**
  ******************************************************************************
  * @brief    This function writes a dynamic number of bytes to FRAM.
  *           This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function uses global variables
  *           to keep track of what bytes have been written to, and than uses the HAL library
  *           to write to the FM24CL16B. 
  *           
  *           The FM24CL16B memory archtecture is structed around 8 segments of 
  *           256 bytes each. This function fills all 256 bytes in one segment before moving on to the next,
  *           it eventually loops back around and starts overwriting at segment 1, byte 1.
  * @param    data An array of data bytes.
  * @param    num_bytes The number of bytes to be written.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
 HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t num_bytes){
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t byte;
    for (int i = 0; i < num_bytes; i++){ // For every byte to be read
        byte = data[i]; // Use individual byte from given array
        status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | page_memory), address_memory, I2C_MEMADD_SIZE_8BIT, &byte, 1, 10); // Write to a paticular segement (by ORing with page memory), and address. Write 8 bits at a time. Timeout after 10ms.
        address_memory += 1; // Iterate address memory
        if (address_memory > MAXIMUM_MEMORY_ADDRESS) { // If segment is full, move onto the next one
            page_memory += 1;
            address_memory = 0;
        }
        if (page_memory > MAXIMUM_PAGE_ADDRESS) { // If all segments are full, begin to overwrite from the beginning
            page_memory = 0;
        }
    }
    return status;
 }

 /**
  ******************************************************************************
  * @brief    This function reads a dynamic number of bytes to FRAM.
  *           This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function uses global variables
  *           to keep track of what bytes have been read from, and than uses the HAL library
  *           to read from the FM24CL16B.
  * @param    data Array to be read into
  * @param    num_bytes The number of bytes to be read.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
HAL_StatusTypeDef FRAM_Read(uint8_t *data, uint8_t num_bytes) {
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t byte;
    for (int i = 0; i < num_bytes; i++){ 
        status = HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | page_memory), address_memory, I2C_MEMADD_SIZE_8BIT, &byte, 1, 10);
        data[i] = byte;
        address_memory -= 1;
        // Will need to think about segment below more
        if (address_memory < 0) { // If segment is full, move back by 1
            page_memory -= 1;
            address_memory = MAXIMUM_MEMORY_ADDRESS;
        }
        if (page_memory > MINIMUM_PAGE_ADDRES) { // If all segments are full, begin to overwrite from the beginning
            page_memory = MAXIMUM_PAGE_ADDRESS;
            address_memory = MAXIMUM_MEMORY_ADDRESS;
        }
    }
    return status;
 }