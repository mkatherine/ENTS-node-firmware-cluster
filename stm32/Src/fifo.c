/**
  ******************************************************************************
  * @file    fifo.c
  * @author  Stephen Taylor
  * @brief   Soil Power Sensor FRAM library
  *          This file provides functions to push/pop to the onboard FM24CL16B 
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
#include "fifo.h"

int write_address_memory = 0x00;
int write_page_memory = 0x00;
int read_address_memory = 0x00;
int read_page_memory = 0x00;


HAL_StatusTypeDef push(const uint8_t *data, uint8_t num_bytes){
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t byte;
    for (int i = 0; i < num_bytes; i++){ // For every byte to be read
        byte = data[i]; // Use individual byte from given array
        status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | write_page_memory), write_address_memory, I2C_MEMADD_SIZE_8BIT, &byte, 1, 10); // Write to a paticular segement (by ORing with page memory), and address. Write 8 bits at a time. Timeout after 10ms.
        write_address_memory += 1; // Iterate address memory
        if (write_address_memory > MAXIMUM_MEMORY_ADDRESS) { // If segment is full, move onto the next page
            write_page_memory += 1;
            write_address_memory = 0;
        }
        if (write_page_memory > MAXIMUM_PAGE_ADDRESS) { // If all segments are full, begin to overwrite from the first page
            write_page_memory = 0;
        }
    }
    return status;
 }

HAL_StatusTypeDef pop(uint8_t *data, uint8_t num_bytes) {
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t byte;
    for (int i = 0; i < num_bytes; i++){ 
        status = HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | read_page_memory), read_address_memory, I2C_MEMADD_SIZE_8BIT, &byte, 1, 10); // Read a byte of data
        data[i] = byte; // Add the byte to the array
        read_address_memory += 1; // Move memory address pointer forward
        // Will need to think about segment below more
        if (read_address_memory > MAXIMUM_MEMORY_ADDRESS) { // If segment is full, move forward by 1 page
            read_page_memory += 1;
            read_address_memory = 0;
        }
        if (read_page_memory > MAXIMUM_PAGE_ADDRESS) { // If all segments are full, begin to overwrite from the first page
            read_page_memory = 0;
        }
    }
    return status;
 }

