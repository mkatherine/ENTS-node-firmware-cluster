/**
 ******************************************************************************
 * @file    sdi12.c
 * @author  Stephen Taylor
 * @brief   This file provides a library to communicate between a STM32 MC
 *          and a SDI-12 device.
 * @date    1/30/2024
 *
 ******************************************************************************
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdi12.h"

// Start LPTIM for delay generation
void LPTIM_Delay_Start() {
  HAL_LPTIM_Counter_Start_IT(&hlptim1, 0xFFFF); // Counter value doesn't matter
}

// Delay function using LPTIM
void LPTIM_Delay(uint32_t delay_ms) {
  // Start LPTIM for delay generation
  LPTIM_Delay_Start();
  // Calculate number of ticks required for the delay
  //uint32_t ticks = (delay_ms * HAL_RCC_GetPCLK1Freq()) / (hlptim.Init.Clock.Prescaler * 1000);
  // Get current tick count from LPTIM counter register
  uint32_t start_tick = LPTIM1->CNT;
  uint32_t ticks = LPTIM1->CNT;
  // Wait until the specified number of ticks has passed
  while ((ticks - start_tick) < 148){
    ticks = LPTIM1->CNT;
  }
  // Stop LPTIM
  HAL_LPTIM_Counter_Stop_IT(&hlptim1); 
}


void SDI12_WakeSensors(void){
    HAL_LIN_SendBreak(&huart2); // Send a break 
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // Send the marking 
    for (int i = 0; i <= 20000; i++){ // Figure out a way to do this delay with the low-power timer
      asm("nop");
    };
    //LPTIM_Delay(9);

}

HAL_StatusTypeDef SDI12_SendCommand(const char *command, uint8_t size){
    SDI12_WakeSensors();
    return HAL_UART_Transmit(&huart2, (const uint8_t *) command, size, 1000);
}

HAL_StatusTypeDef SDI12_ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // Set to RX mode
    __HAL_UART_FLUSH_DRREGISTER(&huart2);
    HAL_UART_Receive(&huart2,(uint8_t *) buffer, bufferSize, timeoutMillis);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // Set to TX mode
    return HAL_OK;
}