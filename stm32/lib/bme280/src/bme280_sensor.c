#include "bme280.h"

/**
 * @brief Constant chip id used in self test
 * 
 */
static const uint8_t chip_id = 0x60;

/**
 * @brief Value written to reset register to soft reset device
 * 
 */
static const uint8_t soft_reset = 0xb6;

/**
 * @brief Reset timeout 
 * 
 * 2ms timeout for the device to reset
 */
static const uint8_t reset_timeout = 2;

/**
 * @brief Write timeout
 * 
 * 10ms timeout for device to ACK a write operation
 * 
 */
static const uint8_t read_timeout = 10;

/**
 * @brief I2C device address
 * 
 * Possible values are 0x77 (default) or 0x76
 */
static uint8_t device_addr = 0x77;

/**
 * @brief Memory map of BME280
 * 
 * The min and max calibration registers are provided. Intermidiate registers are
 * to be determined pragmatically.
 */
typedef enum {
  hum_lsb = 0xfe,
  hum_msb = 0xfd,
  temp_xlsb = 0xfc,
  temp_lsb = 0xfb,
  temp_msb = 0xfa,
  press_xlsb = 0xf9,
  press_lsb = 0xf8,
  press_msb = 0xf7,
  config = 0xf5,
  ctrl_meas = 0xf4,
  status = 0xf3,
  ctrl_hum = 0xf2,
  calib41 = 0xf0,
  calib26 = 0xe1,
  reset = 0xe0,
  id = 0xd0,
  calib25 = 0xa1,
  calib00 = 0x88
} MemoryMap;

/**
 * @brief Preforms a device self test
 * 
 * @return Status code
 */
BME280Status SelfTest(void);

/**
 * @brief Soft reset the sensor
 * 
 * @return Status code
 */
BME280Status SoftReset(void);



BME280Status SelfTest(void) {
  BME280Status status = BME280_OK;
  status = SoftReset();

  uint8_t reg_addr = id;
  HAL_I2C_Master_Transmit(&hi2c2, device_addr, &reg_addr, 1, HAL_MAX_DELAY);

  return BME280_OK;
}

BME280Status SoftReset(void) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t reg_addr;
  status = HAL_I2C_Master_Transmit(&hi2c2, device_addr, &reg_addr, 1,
                                   reset_timeout);

  if (status != HAL_OK) {
    return BME280_ERROR;
  }

  return BME280_OK;
}