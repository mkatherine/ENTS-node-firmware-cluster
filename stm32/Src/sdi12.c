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

typedef struct
{
  char Address;
  uint16_t Time;
  uint8_t NumValues;
} SDI12_Measure_TypeDef;

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

HAL_StatusTypeDef ParseMeasurementResponse(const char *responseBuffer, char addr, SDI12_Measure_TypeDef *measurement_info)
{
  char responseAddr;
  sscanf(responseBuffer, "%1c%3hu%1hhu", &responseAddr, &(measurement_info->Time), &(measurement_info->NumValues)); // Parse the response and populate the structure
  measurement_info->Address = addr; // Update address
  // Check if the response address matches the expected address
  if (responseAddr == addr)
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
    char responseBuffer[MAX_RESPONSE_SIZE]; // Buffer to store the response    char responseAddr; // Address in sensor response
    char sendData[MAX_RESPONSE_SIZE];       // Command to send the data
    HAL_StatusTypeDef ret;

    uint8_t size = sprintf(reqMeas, "%cM!", addr); // Construct a command to request a measurment

    SDI12_SendCommand(reqMeas, size); // Request a measurment

    ret = SDI12_ReadData(responseBuffer, sizeof(responseBuffer), timeoutMillis); // Read the data
    if (ret != HAL_OK){
        return ret;
    }

    size = sprintf(sendData, "%cD0!", addr); // Construct a command to send the data

    ret = ParseMeasurementResponse(responseBuffer, addr, measurment_info); // Patse the data

    if (measurment_info->Time == 0){ // If data is ready now
        SDI12_SendCommand(sendData, size); 
        ret = SDI12_ReadData_V1(measurement_data, SEND_DATA_RESPONSE_SIZE + measurment_info->NumValues, timeoutMillis); // hard coded for a teros measurment response size
        return ret;
    }
}