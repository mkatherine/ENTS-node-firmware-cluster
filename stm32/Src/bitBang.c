/**
 ******************************************************************************
 * @file    bitbang.c
 * @author  Stephen Taylor
 * @brief   This file provides a library to toggle a GPIO using bitbanging.
 * @date    1/30/2024
 *
 ******************************************************************************
 **/

#include <stdio.h>
#include <stdlib.h>

#include "bitbang.h"
#include "tim.h"
#include "usart.h"

void delayMicroseconds(uint32_t microseconds)
{
  //uint32_t timerTicks = microseconds * (FOUR_MHZ / HZ_TO_MHZ); // Set the period of Timer 2
  uint32_t timerTicks = 3332;
  __HAL_TIM_SET_AUTORELOAD(&htim2, timerTicks);
 // __HAL_TIM_SET_COUNTER(&htim2, 0); // Clear the counter value of Timer 2
  HAL_TIM_Base_Start(&htim2); // Start the timer
  while (__HAL_TIM_GET_COUNTER(&htim2) < timerTicks); // Wait until the counter reaches the target value
 // __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE); // Clear the update event flag
  HAL_TIM_Base_Stop(&htim2); // Stop the timer
}

void simpleDelay(uint16_t microseconds){
  microseconds = microseconds * ONE_COMMAND_IN_MICROSECONDS_AT_FOURTY_EIGHT_MHZ;
  for (uint32_t i = 0; i < microseconds; i++) {
        __asm__("nop");  // This is a no-operation instruction
  }
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
void SendContinousHigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t milliseconds)
{                                                     // Helper function to wake sensors
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);   // Set the data line high
  HAL_Delay(milliseconds);                   // Delay for 12ms according to SDI12 protocol
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
void SendStartBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); // Set the GPIO pin low (start bit)
  simpleDelay(ONE_BIT_IN_MICROSECONDS);
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
void SendStopBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); // Set the GPIO pin high (stop bit)
  simpleDelay(ONE_BIT_IN_MICROSECONDS);
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
void SendCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char character)
{
  for (int i = 0; i < 8; ++i)
  {
    // Send each bit, starting from the least significant bit
    if (character & (1 << i))
    {
      HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); // Set the GPIO pin high for a '1' bit
    }
    else
    {
      HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); // Set the GPIO pin low for a '0' bit
    }
    simpleDelay(ONE_BIT_IN_MICROSECONDS);
  }
  return;
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
char ReadCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  char bit;
  char character = 0;

  for (int i = 0; i < 8; ++i)
  {
    // Read the current bit
    bit = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET ? '1' : '0';

    // Delay for half the expected bit time
    simpleDelay(HALF_BIT_IN_MICROSECONDS);

    // Append the bit to the character
    character |= (bit - '0') << i;
  }

  return character;
}