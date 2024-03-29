/**
 ******************************************************************************
 * The FM24CL16B memory archtecture is structed around 8 segments of
 * 256 bytes each. The measurment data is stored on the first 7 segments in
 * as a FIFO data structure. The 8th page is reserved for user configurable settings
 * with space left over for whatever information the user wishes to store in
 * non-volatile memory.
 ******************************************************************************
 **/

/* Includes ------------------------------------------------------------------*/
#include "fram.h"

/** Max writeable address */
const uint16_t fram_max_addr = FRAM_SEG_SIZE * FRAM_PAGES;

/** Timeout in seconds for the FRAM chip */
const uint32_t fram_timeout = 10;

/** Matrix representation of memory */
typedef struct
{
  /** Page reference */
  uint16_t page;
  /** Segment address */
  uint16_t seg;
} FramAddress;

/**
 * @brief Convert an address to matrix memory representation
 *
 * No out of memory errors are checked.
 *
 * @param addr Memory address
 * @return Matrix representation of address
 */
FramAddress fram_convert_Addr_Mem(uint16_t addr);

/**
 ******************************************************************************
 * The FM24CL16B memory archtecture is structed around 8 segments of
 * 256 bytes each. This function fills all 256 bytes in one segment before moving on to the next,
 * it eventually loops back around and starts overwriting at segment 1, byte 1. Leaving the 8th segment for user
 * configurable settings.
 ******************************************************************************
 */
FramStatus FRAM_Write(uint16_t addr, const uint8_t *data, uint8_t len)
{
  // Write byte array to memory
  // NOTE write is performed a single byte at a time due to address
  // configuration of the chip.
  for (int i = 0; i < len; i++) {
    FramAddress addr_mat = fram_convert_Addr_Mem(addr);

    // check for out of memory
    if (addr_mat.page > FRAM_PAGES)
    {
      return FRAM_OUT_OF_RANGE;
    }

    // Write byte to an address
    // Can optimize by using continuous writes but needs to account for the
    // change of address.
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | addr_mat.page),
                               addr_mat.seg, I2C_MEMADD_SIZE_8BIT,
                               data[i], 1, fram_timeout);
    
    // return error if failed to write
    if (status != HAL_OK) {
      return FRAM_ERROR;
    }

    // increment address
    ++addr;

  }

  return FRAM_SUCCESS;
}

/**
 ******************************************************************************
 * This function is a wrapper for the fifo library. The FM24CL16B uses
 * I2C the I2C communication protocol. This function uses global variables
 * to keep track of what bytes have been read from, and than uses the HAL library
 * to read from the FM24CL16B.
 ******************************************************************************
 */
FramStatus FRAM_Read(uint16_t addr, uint8_t len, uint8_t *data)
{
  // Write byte array to memory
  // NOTE write is performed a single byte at a time due to address
  // configuration of the chip.
  for (int i = 0; i < len; i++) {
    FramAddress addr_mat = fram_convert_Addr_Mem(addr);

    // check for out of memory
    if (addr_mat.page > FRAM_PAGES)
    {
      return FRAM_OUT_OF_RANGE;
    }

    // Write byte to an address
    // Can optimize by using continuous writes but needs to account for the
    // change of address.
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Read(&hi2c2, (FM24_READ | addr_mat.page),
                              addr_mat.seg, I2C_MEMADD_SIZE_8BIT,
                              data, 1, fram_timeout);

    // increment data addr
    ++data;
    
    // return error if failed to write
    if (status != HAL_OK) {
      return FRAM_ERROR;
    }

    // increment address
    ++addr;
  }

  return FRAM_SUCCESS;
}

/**
 ******************************************************************************
 *  This function is a wrapper for the STM32 HAl I2C library. The FM24CL16B uses
 *  I2C the I2C communication protocol. This function stores the cell ID, logger ID, LoRaWAN gateway EUI, LoRaWAN application EUI,
 *  end device EUI, logging interval and upload interval at the hard-coded memory adresses: 0x00, 0x08, 0x10, 0x17, 0x1f, 0x27 and 0x29 respectivley. This data is stored on the FM24CL16B's
 *  8th page of memory.
 ******************************************************************************
 */
HAL_StatusTypeDef configure_Settings(configuration c)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), CELL_ID_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.cell_ID, 8, 10); // Store the cell ID
  if (status != HAL_OK)
  {
    return status;
  }
  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LOGGER_ID_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.logger_ID, 8, 10); // Store the logger ID
  if (status != HAL_OK)
  {
    return status;
  }
  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LORA_GATEWAY_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.gateway_EUI, 8, 10); // Store the gateway eui
  if (status != HAL_OK)
  {
    return status;
  }
  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LORA_APPLICATION_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.application_EUI, 8, 10); // Store the application eui
  if (status != HAL_OK)
  {
    return status;
  }
  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LORA_END_DEVICE_EUI_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.end_device_EUI, 8, 10); // Store the application eui
  if (status != HAL_OK)
  {
    return status;
  }
  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), LOGGING_INTERVAL_IN_SECONDS_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.logging_interval, 2, 10); // Store the logging interval
  if (status != HAL_OK)
  {
    return status;
  }
  status = HAL_I2C_Mem_Write(&hi2c2, (FM24_WRITE | USER_DATA_PAGE_ADDRESS), UPLOAD_INTERVAL_IN_MINUTES_MEMORY_ADDRESS, I2C_MEMADD_SIZE_8BIT, &c.upload_interval, 2, 10); // Store the upload interval
  if (status != HAL_OK)
  {
    return status;
  }
  return status;
}

configuration read_Settings(void)
{
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

FramAddress fram_convert_Addr_Mem(uint16_t addr)
{
  // convert flat address space to matrix
  FramAddress addr_mat;
  addr_mat.page = addr / FRAM_PAGES;
  addr_mat.seg = addr % FRAM_SEG_SIZE;

  return addr_mat;
}
