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

int write_address_memory = 0x00;
int write_page_memory = 0x00;
int read_address_memory = 0x00;
int read_page_memory = 0x00;


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
HAL_StatusTypeDef FRAM_Write(const uint8_t *data, uint8_t num_bytes){
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t byte;
    for (int i = 0; i < num_bytes; i++){ // For every byte to be read
        byte = data[i]; // Use individual byte from given array
        status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | write_page_memory), write_address_memory, I2C_MEMADD_SIZE_8BIT, &byte, 1, 10); // Write to a paticular segement (by ORing with page memory), and address. Write 8 bits at a time. Timeout after 10ms.
        write_address_memory += 1; // Iterate address memory
        if (write_address_memory > MAXIMUM_MEMORY_ADDRESS) { // If segment is full, move onto the next one
            write_page_memory += 1;
            write_address_memory = 0;
        }
        if (write_page_memory > MAXIMUM_PAGE_ADDRESS) { // If all segments are full, begin to overwrite from the beginning
            write_page_memory = 0;
        }
    }
    return status;
 }

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
HAL_StatusTypeDef FRAM_Read(uint8_t *data, uint8_t num_bytes) {
    const char * success = "FRAM responded\n";
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t byte;
    for (int i = 0; i < num_bytes; i++){ 
        status = HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | read_page_memory), read_address_memory, I2C_MEMADD_SIZE_8BIT, &byte, 1, 10);
        data[i] = byte;
        read_address_memory += 1;
        // Will need to think about segment below more
        if (read_address_memory > MAXIMUM_MEMORY_ADDRESS) { // If segment is full, move forward by 1
            read_page_memory += 1;
            read_address_memory = 0;
        }
        if (read_page_memory > MAXIMUM_PAGE_ADDRESS) { // If all segments are full, begin to overwrite from the beginning
            read_page_memory = 0;
        }
    }
    return status;
 }

/**
  ******************************************************************************
  * @brief    This function stores user configurable settings to non-volatile memory.
  *           Specifically cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application EUI and 
  *           end device EUI.
  * 
  *           This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
  *           I2C the I2C communication protocol. This function stores the cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application EUI and 
  *           end device EUI at the hard-coded memory adresses: 0x00, 0x08, 0x10, 0x17 and 0x1f respectivley. This data is stored on the FM24CL16B's 
  *           8th page of memory.
  *           
  * @param    cell_ID The 64 bit microbial fuel cell identifier. Pass in hex format.
  * @param    logger_ID The 64 bit logger identifier. Pass in hex format.
  * @param    gateway_EUI The 64 bit LoRaWAN gateway extended unique identifier. Pass in hex format.
  * @param    application_EUI The 64 bit LoRaWAN application extended unique identifier. Pass in hex format.
  * @param    end_device_EUI The 64 bit LoRaWAN end-device extended unique identifier. Pass in hex format.
  * @return   HAL_StatusTypeDef, status of the I2C function
  ******************************************************************************
  */
 HAL_StatusTypeDef configure_Settings(uint64_t cell_ID, uint64_t logger_ID, uint64_t gateway_EUI, uint64_t application_EUI, uint64_t end_device_EUI){
    HAL_StatusTypeDef status = HAL_OK;

    for (int i = 0; i < 8; i++){
        status = HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | USER_DATA_PAGE_ADDRESS), (CELL_ID_MEMORY_ADDRESS + i), I2C_MEMADD_SIZE_8BIT, &byte, 1, 10); // Store 1 byte of the cell ID
        if (status != HAL_OK){ return status; }
    }
 }