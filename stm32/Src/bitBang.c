/**
  ******************************************************************************
  * @file    bitbang.c
  * @author  Stephen Taylor
  * @brief   This file provides a library to toggle a GPIO using bitbanging.
  * @date    1/30/2024
  *
  ******************************************************************************
  **/

 #include "bitbang.h"

/**
  ******************************************************************************
  * @brief    This is a helper function to wake all the sensors.
  * 
  * @param    GPIO_TypeDef *GPIOx, an instance of the typdef struct GPIO_Typedef
  * @param    uint16_t, GPIO_Pin
  * @return   void
  ******************************************************************************
  */
 void wake_Sensors(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){ // Helper function to wake sensors
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); // Set the data line low
    HAL_Delay(WAKE_SENSOR_DELAY);
 }

/**
  ******************************************************************************
  * @brief    This is a helper function to send a start bit.
  * 
  * @param    void
  * @return   void
  ******************************************************************************
  */
void SDI12_SendStartBitSDI12_SendStartBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);  // Set the GPIO pin low (start bit)
    HAL_Delay(10);  // Adjust the delay as needed based on your baud rate
}

// Function to send a stop bit
void SDI12_SendStopBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // Set the GPIO pin high (stop bit)
    HAL_Delay(10);  // Adjust the delay as needed based on your baud rate
}

// Function to send a single character
void SDI12_SendCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char character) {
    for (int i = 0; i < 8; ++i) {
        // Send each bit, starting from the least significant bit
        if (character & (1 << i)) {
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);  // Set the GPIO pin high for a '1' bit
        } else {
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // Set the GPIO pin low for a '0' bit
        }

        HAL_Delay(10);  // Adjust the delay as needed based on your baud rate
    }
}