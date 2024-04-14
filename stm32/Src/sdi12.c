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

void LPTIM_Delay_ms(uint32_t milliseconds) {
    // Calculate the number of timer counts for specic pulse
    uint32_t pulseCounts = milliseconds * LPTIM_MS_TO_TICKS;   

    // Start the LPTIM in counter Mode
    
    
      // Wait until the pulse is complete
    // Get the current value of the LPTIM counter
    uint32_t start_tick = HAL_LPTIM_ReadCounter(&hlptim1);

    // Calculate the target value of the LPTIM counter after the delay
    uint32_t target_tick = start_tick + pulseCounts;

    // Monitor the counter until the specified number of milliseconds have passed
    while (HAL_LPTIM_ReadCounter(&hlptim1) < target_tick) {
        // Wait
    }
    
}

void TIM16_Delay_ms(uint32_t milliseconds) {
    // Calculate the number of timer counts for specific pulse
    
    HAL_TIM_Base_Start(&htim16);
    // Get the current value of the TIM16 counter
    uint32_t start_tick = __HAL_TIM_GET_COUNTER(&htim16);

    // Calculate the maximum target value of the TIM16 counter after the delay
    uint32_t max_target_tick = 65535;

    // Calculate the target value of the TIM16 counter after the delay
    uint32_t target_tick = start_tick + milliseconds;

    // Monitor the counter until the specified number of milliseconds have passed
    while (__HAL_TIM_GET_COUNTER(&htim16) < target_tick) {
        // Check for overflow
        if (target_tick > max_target_tick) {
            // Calculate the remaining counts after overflow
            uint32_t remaining_counts = target_tick - max_target_tick;
            // Wait until the remaining counts have also elapsed
            while (__HAL_TIM_GET_COUNTER(&htim16) < remaining_counts) {
                // Wait
            }
            // Exit the loop after handling overflow
            break;
        }
    }
    HAL_TIM_Base_Stop(&htim16);
}


void SDI12_WakeSensors(void){
    HAL_LIN_SendBreak(&huart2); // Send a break 
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // Send the marking 
    // for (int i = 0; i <= 20000; i++){ // Figure out a way to do this delay with the low-power timer
    //   asm("nop");
    // };
    TIM16_Delay_ms(10);

}

HAL_StatusTypeDef SDI12_SendCommand(const char *command, uint8_t size){
    HAL_StatusTypeDef ret;
    SDI12_WakeSensors();
    ret = HAL_UART_Transmit(&huart2, (const uint8_t *) command, size, 1000);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // Set to RX mode
    return ret;
}

HAL_StatusTypeDef SDI12_ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // Set to RX mode
    __HAL_UART_FLUSH_DRREGISTER(&huart2);
    HAL_UART_Receive(&huart2,(uint8_t *) buffer, bufferSize, HAL_MAX_DELAY);
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // Set to TX mode
    return HAL_OK;
}

HAL_StatusTypeDef ParseMeasurementResponse(const char *responseBuffer, char addr, SDI12_Measure_TypeDef *measurement_info)
{
  sscanf(responseBuffer, "%1c%3hu%1hhu", &(measurement_info->Address), &(measurement_info->Time), &(measurement_info->NumValues)); // Parse the response and populate the structure
  // Check if the response address matches the expected address
  if (measurement_info->Address == addr)
  {
    return HAL_OK; // Return success
  }
  else
  {
    return HAL_ERROR; // Return error - Address mismatch
  }
  return HAL_OK;
}

HAL_StatusTypeDef ParseServiceRequest(const char *requestBuffer, char addr)
{
  char expectedResponse[12];
  sprintf(expectedResponse, "%c\r\n", addr); // Construct the expected response ("a\r\n")
  // Parse the service request
  if (memcmp(requestBuffer, expectedResponse, 3) == 0) // Check if the response matches the expected Active Acknowledge response
  {
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

HAL_StatusTypeDef SDI12_GetMeasurment(uint8_t addr, SDI12_Measure_TypeDef *measurment_info, char *measurment_data, uint16_t timeoutMillis)
{
    char reqMeas[MAX_RESPONSE_SIZE];        // Command to request measurement ("0!\r\n" for example)
    char sendData[MAX_RESPONSE_SIZE];       // Command to send the data
    HAL_StatusTypeDef ret;

    uint8_t size = sprintf(reqMeas, "%cM!", addr); // Construct a command to request a measurment

    char buffer[7];
    SDI12_SendCommand(reqMeas, size); // Request a measurment and read the response
    ret = SDI12_ReadData(buffer, 7, timeoutMillis);
    if (ret != HAL_OK){
      return ret;
    }

    size = sprintf(sendData, "%cD0!", addr); // Construct a command to send the data

    ret = ParseMeasurementResponse(buffer, addr, measurment_info); // Check if the addresses match from the response above, the response from a teros is the same every time so we're going to leave it for now
    if (ret != HAL_OK){
        return ret;
    }

    if (measurment_info->Time == 0){ // If data is ready now
        SDI12_SendCommand(sendData, size); 
        ret = SDI12_ReadData(measurment_data, SEND_DATA_RESPONSE_SIZE + measurment_info->NumValues, timeoutMillis); // hard coded for a teros measurment response size
        return ret;
    }

    ret = SDI12_ReadData(buffer, 3, timeoutMillis); // Read the service request, wait's for one to arrive
    if (ret != HAL_OK)
    {
      return ret;
    }
    ret = ParseServiceRequest(buffer, measurment_info->Address); // make sure the service request is correct
    if (ret != HAL_OK)
    {
      return ret;
    }
    SDI12_SendCommand(sendData, size);
    ret = SDI12_ReadData(measurment_data, 18, timeoutMillis); // IT IS VERY IMPORTANT THAT THE AMOUNT OF DATA EXPECTED TO BE READ (bufferSize) MATCH WHAT IS SENT EXACTLY. Or maybe slightly smaller, but any greater causes the program to hang
    return ret;
}