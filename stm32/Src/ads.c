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
* @param    void
* @return   HAL_StatusTypeDef
******************************************************************************
*/
HAL_StatusTypeDef ADC_init(void){
    uint8_t code[2];
    uint8_t ca[4];
    uint8_t read_reg = 0x20;
    uint8_t creg;
    HAL_StatusTypeDef ret;
    // Control register breakdown.
    //  7:5 MUX (default)
    //  4   Gain (default)
    //  3:2 Data rate (default)
    //  1   Conversion mode (default)
    //  0   VREF (External reference 3.3V)
    const unsigned char * rM = "reset failed\r\n";
    code[0] = 0x0;
    code[1] = 0x6;
    // const unsigned char * cM = "writing to control reg failed\r\n";
    // const unsigned char * rcM = "writing to read cReg failed\r\n";
    // const unsigned char * rrM = "reading cReg failed\r\n";
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Power down pin has to be set to high before any of the analog circuitry can function
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, code, 2, 10);  // Send the reset code
    if (ret != HAL_OK){
      HAL_UART_Transmit(&huart1, rM, 15, 19);
      //return ret;
    }

    //code = ADS12_WRITE_TO_REGISTER; // Set the control register, leaving everything at default except for the VREF, which will be set to external reference mode
    ca[0] = 0x2;
    ca[1] = 0xE;
    ca[2] = 0x0;
    ca[3] = 0x4;
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, ca, 2, 10);
    if (ret != HAL_OK){
     // HAL_UART_Transmit(&huart1, cM, 32, 19);
      //return ret;
    }
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &read_reg, 1, 10);
    if (ret != HAL_OK){
      //HAL_UART_Transmit(&huart1, rcM, 30, 19);
      //return ret;
    }
    ret = HAL_I2C_Master_Receive(&hi2c2, ADS12_READ, &creg, 1, 10);
    //char output[10];
    //sprintf(output, "C Reg: %d", creg);
   // HAL_UART_Transmit(&huart1, output, 10, 10);
    if (ret != HAL_OK){
      //HAL_UART_Transmit(&huart1, rrM, 9, 19);
      //return ret;
    }
    //code = ADS12_START_CODE;
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, 10); // Send a start code
    return ret;
 }

/**
******************************************************************************
* @brief    This function reads the current ADC value.
* 
*           This function is a wrapper for the STM32 HAl I2C library. The ADS1219 uses
*           I2C the I2C communication protocol. 
*           
* @param    void
* @return   int, current ADC reading
******************************************************************************
*/
int ADC_read(void){
    uint8_t code;
    int reading;
    uint8_t rx_data[3]; // Why is this only 3 bytes?
   // char output[16];
   // const unsigned char * fail_read = "write d tx failed\n";
   // const unsigned char * fail_write = "read d rx failed\n";
    
    while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))); // Wait for the DRDY pin on the ADS12 to go low, this means data is ready
    code = ADS12_READ_DATA_CODE;
    if (HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 2, 10) != HAL_OK){
   //   HAL_UART_Transmit(&huart1, fail_write, 18, 19);
    } // Send a write command that lets the ADS know to send data

    if (HAL_I2C_Master_Receive(&hi2c2, ADS12_READ, rx_data, 3, 10) != HAL_OK){// Recieve the ADS data from
    //  HAL_UART_Transmit(&huart1, fail_read, 19, 19);
    }

    reading = ((int)rx_data[2] << 16) | ((int)rx_data[1] << 8) | rx_data[0];
   // sprintf(output, "Read: %d,%d,%d\n", rx_data[2],rx_data[1],rx_data[0]);
    return reading;
 }

/**
******************************************************************************
* @brief    This function probes the ADS12 to see if it is responsive.
*           
* @param    void
* @return   HAL_StatusTypeDef
******************************************************************************
*/
HAL_StatusTypeDef probeADS12(void){
  HAL_StatusTypeDef ret;
  ret = HAL_I2C_IsDeviceReady(&hi2c2, ADS12_WRITE, 10, 20);
  return ret;
}
