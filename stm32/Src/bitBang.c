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
 #include "tim.h"

void delayMicroseconds(uint32_t microseconds) {
    uint32_t targetCount = __HAL_TIM_GET_COUNTER(&htim2) + microseconds;

    // Wait until the timer reaches the target count
    while (__HAL_TIM_GET_COUNTER(&htim2) < targetCount);
    return;
}

/**
  ******************************************************************************
  * @brief    Send a continous high signal.
  * 
  * @param    GPIO_TypeDef *GPIOx, an instance of the typdef struct GPIO_Typedef
  * @param    uint16_t, GPIO_Pin
  * @param    uint32_t, microseconds
  * @return   void
  ******************************************************************************
  */
 void SendContinousHigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t microseconds){ // Helper function to wake sensors
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); // Set the data line high
    delayMicroseconds(microseconds); // Delay for 12ms according to SDI12 protocol
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); // Set it low again
 }

/**
  ******************************************************************************
  * @brief    This is a helper function to send a start bit.
  * 
  * @param    GPIO_TypeDef, *GPIOx
  * @param    uint16_t GPIO_Pin
  * @return   void
  ******************************************************************************
  */
void SendStartBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);  // Set the GPIO pin low (start bit)
    delayMicroseconds(ONE_BIT_IN_MICROSECONDS);  
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/**
  ******************************************************************************
  * @brief    This is a helper function to send a stop bit.
  * 
  * @param    GPIO_TypeDef, *GPIOx
  * @param    uint16_t GPIO_Pin
  * @return   void
  ******************************************************************************
  */
void SendStopBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);  // Set the GPIO pin high (stop bit)
    delayMicroseconds(ONE_BIT_IN_MICROSECONDS);  
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/**
  ******************************************************************************
  * @brief    Sends a single char off the specified GPIO bus
  * 
  * @param    GPIO_TypeDef, *GPIOx
  * @param    uint16_t GPIO_Pin
  * @return   void
  ******************************************************************************
  */
void SendCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char character) {
    for (int i = 0; i < 8; ++i) {
        // Send each bit, starting from the least significant bit
        if (character & (1 << i)) {
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);  // Set the GPIO pin high for a '1' bit
        } else {
            HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // Set the GPIO pin low for a '0' bit
        }
        delayMicroseconds(ONE_BIT_IN_MICROSECONDS);
    }
}

/**
  ******************************************************************************
  * @brief    Reads a single character of the specified GPIO pin
  * 
  * @param    GPIO_TypeDef, *GPIOx
  * @param    uint16_t GPIO_Pin
  * @return   char
  ******************************************************************************
  */
char ReadCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    char bit;
    char character = 0;

    for (int i = 0; i < 8; ++i) {
        // Read the current bit
        bit = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET ? '1' : '0';

        // Delay for half the expected bit time
        delayMicroseconds(HALF_BIT_IN_MICROSECONDS);

        // Append the bit to the character
        character |= (bit - '0') << i;
    }

    return character;
}