/**
  ******************************************************************************
  * @file    ads.c
  * @author  Stephen Taylor
  * @brief   Soil Power Sensor ADS12 library
  *          This file provides a function to read from the onboard ADC (ADS1219).
  * @date    11/27/2023
  *
  ******************************************************************************
  **/

/* Includes ------------------------------------------------------------------*/
#include "ads.h"

/**
******************************************************************************
* @brief    This function starts up the ADS1219
* 
*           This function is a wrapper for the STM32 HAl I2C library. The ADS1219 uses
*           I2C the I2C communication protocol. This function configures then ADS1219 for
*           single read mode. Note: the ADS1219 requires a minimum of 500us when it is powered on.
*       
******************************************************************************
*/
HAL_StatusTypeDef ADC_init(void){
    uint8_t code = ADS12_RESET_CODE;
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 2, 10);
    return ret;
 }

/**
******************************************************************************
* @brief    This function reads the current ADC value.
* 
*           This function is a wrapper for the STM32 HAl I2C library. The ADS1219 uses
*           I2C the I2C communication protocol. 
*           
* @return   int, current ADC reading
******************************************************************************
*/

int ADC_read(){
    uint8_t code;
    int reading = 0;
    uint8_t rx_data[3]; // Why is this only 3 bytes?

    code = ADS12_START_CODE;
    HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 2, 10); // Send a start code
    while(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))); // Wait for the DRDY pin on the ADS12 to go low, this means data is ready
    code = ADS12_READ_DATA_CODE;
    HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 2, 10); // Send a write command that lets the ADS know to send data
    HAL_I2C_Master_Receive(&hi2c2, ADS12_READ, rx_data, 3, 10); // Recieve the ADS data from
    // Next steps, combine uint8_ts into single int
    // Return single int
 }
