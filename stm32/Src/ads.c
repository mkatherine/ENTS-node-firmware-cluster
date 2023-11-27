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
    uint8_t rx_data[3];

    code = ADS12_STARTUP_CODE;
    HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 2, 10);
    code = ADS12_READ_DATA_CODE;
    HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 2, 10);
    HAL_I2C_Master_Receive(&hi2c2, ADS12_READ, rx_data, 3, 10);
 }
