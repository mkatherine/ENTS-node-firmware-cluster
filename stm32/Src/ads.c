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
#include "usart.h"

int HAL_status(HAL_StatusTypeDef ret) {
  int status;
  if (ret == HAL_OK){
    status = 0;
  } else if (ret == HAL_ERROR){
    status = 1;
  } else if (ret == HAL_BUSY){
    status = 2;
  } else {
    status = 3;
  }
  return status;
}

HAL_StatusTypeDef configurADS(uint8_t config){
  HAL_StatusTypeDef ret;
  uint8_t code = ADS12_RESET_CODE;
  uint8_t register_data[2] = {0x40, config}; // Initialize to voltage read first

  ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY);  // Send the reset code
  if (ret != HAL_OK){
      return ret;
  } 
  // Set the control register, leaving everything at default except for the VREF, which will be set to external reference mode
  ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, register_data, 2, HAL_MAX_DELAY);
  if (ret != HAL_OK){
    return ret;
  }

  code = ADS12_START_CODE;
  ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY); // Send a start code
  if (ret != HAL_OK){
    return ret;
  }
  //HAL_Delay(100);
  return HAL_OK;
}

HAL_StatusTypeDef ADC_init(void){
    uint8_t code = ADS12_RESET_CODE;
    uint8_t register_data[2] = {0x40, 0x01}; // Initialize to voltage read first
    HAL_StatusTypeDef ret;

    // Control register breakdown.
    //  7:5 MUX 
    //  4   Gain 
    //  3:2 Data rate 
    //  1   Conversion mode 
    //  0   VREF (External reference 3.3V)

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Power down pin has to be set to high before any of the analog circuitry can function
    // delay after startup 
    for (int i=0; i < 1000; ++i); __NOP();
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY);  // Send the reset code
    if (ret != HAL_OK){
      return ret;
    } 

    // Set the control register, leaving everything at default except for the VREF, which will be set to external reference mode
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, register_data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK){
      return ret;
    }
    
    code = ADS12_START_CODE;
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY); // Send a start code
    if (ret != HAL_OK){
      return ret;
    }

    return ret;
 }

int ADC_readVoltage(void){
    uint8_t code;
    int16_t reading;
    HAL_StatusTypeDef ret;
    uint8_t rx_data[3] = {0x00, 0x00, 0x00}; 
    char hal_fail[] = "hal failed\r\n";
    char raw[100];

    // ret = configurADS(ADS12_CONFIGURE_READ_VOLTAGE); //Set the control register to read voltage mode, leaving everything at default except for the VREF, which will be set to external reference mode
    // if (ret != HAL_OK){
    //   HAL_UART_Transmit(&huart1, hal_fail, 13, 100);
    //   return ret;
    // }
    
    while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))); // Wait for the DRDY pin on the ADS12 to go low, this means data is ready
    code = ADS12_READ_DATA_CODE;
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY);
    if (ret != HAL_OK){
      HAL_UART_Transmit(&huart1, hal_fail, 13, 100);
      return ret;
    }

    ret = HAL_I2C_Master_Receive(&hi2c2, ADS12_READ, rx_data, 3, 1000);
    if (ret != HAL_OK){// Recieve the ADS data from
      HAL_UART_Transmit(&huart1, hal_fail, 13, 100);
      return ret;
    }
    int len = sprintf(raw, "%x %x %x\r\n", rx_data[0], rx_data[1], rx_data[2]);
    HAL_UART_Transmit(&huart1, raw, len, 100);
    
    //reading = ((int)rx_data[0] << 16) | ((int)rx_data[1] << 8) | (int)rx_data[0]; // Use this line if you want to keep the last byte
    reading = ((int)rx_data[0] << 8) | (int)rx_data[1]; // Chop the last byte, as it seems to be mostly noise

    // Uncomment these lines if you wish to see the raw and shifted values from the ADC for calibration purpouses
    // You will have to use these lines to get the raw x values to plug into the linear_regression.py file
    // char raw[45];
    // sprintf(raw, "Raw: %x %x  Shifted: %i \r\n\r\n",rx_data[0], rx_data[1], reading);
    // HAL_UART_Transmit(&huart1, (const uint8_t *) raw, 36, 19);

    reading =  (VOLTAGE_SLOPE * reading) + VOLTAGE_B; // Calculated from linear regression
    return reading;
 }
// Currently never leaves the -6600 to -6700 raw value. I suspect becuase the current draw on the device is so low
int ADC_readCurrent(void){
    uint8_t code;
    int16_t reading;
    HAL_StatusTypeDef ret;
    uint8_t rx_data[3] = {0x00, 0x00, 0x00}; 

    ret = configurADS(ADS12_CONFIGURE_READ_CURRENT); //Set the control register to read current mode, leaving everything at default except for the VREF, which will be set to external reference mode
    if (ret != HAL_OK){
      return ret;
    }

    code = ADS12_START_CODE;
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY); // Send a start code
    if (ret != HAL_OK){
      return ret;
    }
    
    while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3))); // Wait for the DRDY pin on the ADS12 to go low, this means data is ready
    code = ADS12_READ_DATA_CODE;
    ret = HAL_I2C_Master_Transmit(&hi2c2, ADS12_WRITE, &code, 1, HAL_MAX_DELAY);
    if (ret != HAL_OK){
      return ret;
    }

    ret = HAL_I2C_Master_Receive(&hi2c2, ADS12_READ, rx_data, 3, 1000);
    if (ret != HAL_OK){// Recieve the ADS data from
      return ret;
    }
    
    //reading = ((int)rx_data[0] << 16) | ((int)rx_data[1] << 8) | (int)rx_data[2]; // Use this line if you want to keep the last byte
    reading = ((int)rx_data[0] << 8) | (int)rx_data[1]; // Chop the last byte, as it seems to be mostly noise

    // Uncomment these lines if you wish to see the raw and shifted values from the ADC for calibration purpouses
    // You will have to use these lines to get the raw x values to plug into the linear_regression.py file
    // char raw[45];
    // sprintf(raw, "RawC: %x %x %x ShiftedC: %i \r\n\r\n", rx_data[0], rx_data[1], rx_data[2], reading);
    // HAL_UART_Transmit(&huart1, (const uint8_t *) raw, 31, 19);

   // reading =  (CURRENT_SLOPE * reading) + CURRENT_B; // Calculated from linear regression
    return reading;
 }

HAL_StatusTypeDef ADC_probe(void){
  HAL_StatusTypeDef ret;
  ret = HAL_I2C_IsDeviceReady(&hi2c2, ADS12_WRITE, 10, 20);
  return ret;
}

int ADC_filter(int readings[], int size){
  int filtered_reading = 0;
  for(int i = 0; i < size; i++){
    filtered_reading = filtered_reading + readings[i];
  }
  filtered_reading = filtered_reading / size;
  return filtered_reading;
}
