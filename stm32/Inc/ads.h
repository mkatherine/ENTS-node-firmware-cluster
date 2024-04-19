/**
 ******************************************************************************
 * @file     ads.h
 * @author   Stephen Taylor
 * @brief    This file contains all the function prototypes for
 *           the ads.c file
 * @date     11/27/2023
 ******************************************************************************
 */

#ifndef __ADS_H__
#define __ADS_H__

#include <stdio.h>

#include "i2c.h"
#include "usart.h"
#include "stm32_systime.h"

#include "user_config.h"
#include "timestamp.h"
#include "transcoder.h"

#define ADS12_WRITE 0x80
#define ADS12_READ 0x81

#define ADS12_START_CODE 0x08
#define ADS12_RESET_CODE 0x06
#define ADS12_POWERDOWN_CODE 0x02
#define ADS12_READ_DATA_CODE 0x10
#define ADS12_WRITE_TO_REGISTER 0x40E2
#define ADS12_READ_CONTROL_REGISTER 0x20
#define ADS12_CONTROL_REGISTER 0b00000001
#define ADS12_CONFIGURE_READ_VOLTAGE 0x01
#define ADS12_CONFIGURE_READ_CURRENT 0x21

#define TWO_POW_TWENTYTHREE 8388608 // Eqn 5 in ADS1219 datasheet

#define VOLTAGE_SLOPE -0.1 // Change this to the slope output from the linear_regression.py file
#define VOLTAGE_B 6575 // Change this to the b output from the linear_regression.py file

#define CURRENT_SLOPE 0
#define CURRENT_B 0

#ifdef __cplusplus
extern "C"
{
#endif

/**
******************************************************************************
* @brief    This function starts up the ADS1219
* 
*           This function is a wrapper for the STM32 HAl I2C library. The ADS1219 uses
*           I2C the I2C communication protocol. This function configures then ADS1219 for
*           single read mode. Note: the ADS1219 requires a minimum of 500us when it is powered on.
*
* @param    void
* @return   HAL_StatusTypeDef
******************************************************************************
*/
HAL_StatusTypeDef ADC_init(void);

HAL_StatusTypeDef ADC_configure(uint8_t reg_data);

/**
******************************************************************************
* @brief    This function reads the current ADC voltage value.
* 
*           This function is a wrapper for the STM32 HAl I2C library. The ADS1219 uses
*           I2C the I2C communication protocol. This version simply chops the noisy bits.
*           
* @param    void
* @return   double, current ADC reading in microvolts
******************************************************************************
*/
double ADC_readVoltage(void);

/**
******************************************************************************
* @brief    This function reads the current ADC ampere value.
* 
*           This function is a wrapper for the STM32 HAl I2C library. The ADS1219 uses
*           I2C the I2C communication protocol. This version simply chops the noisy bits.
*           
* @param    void
* @return   double, current ADC reading in microamps
******************************************************************************
*/
double ADC_readCurrent(void);

/**
******************************************************************************
* @brief    This function probes the ADS12 to see if it is responsive.
*           
* @param    void
* @return   HAL_StatusTypeDef
*******************************************f***********************************
*/
HAL_StatusTypeDef ADC_probe(void);

/* USER CODE END 1 */