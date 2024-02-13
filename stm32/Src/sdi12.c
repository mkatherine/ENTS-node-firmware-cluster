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

SDI12_TypeDef sdi12;

HAL_StatusTypeDef ParseMeasurementResponse(const char *responseBuffer, char addr, SDI12_Measure_TypeDef *measurement_info)
{
  char responseAddr;
  // Parse the response and populate the structure
  if (sscanf(responseBuffer, "%1c%3hu%1hhu", &responseAddr, &(measurement_info->Time), &(measurement_info->NumValues)) == 3)
  {
    // Successfully parsed the response, update other fields
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
  }
  else
  {
    // Failed to parse the response
    return HAL_ERROR; // Return error
  }
}

HAL_StatusTypeDef ParseServiceRequest(const char *requestBuffer, char addr)
{
  char responseAddr;
  // Parse the service request
  if (sscanf(requestBuffer, "%c\r\n", &responseAddr) == 1)
  {
    // Check if the format matches a service request
    // No need to check newline characters separately, sscanf already verifies them
    if (responseAddr == addr)
    {
      return HAL_OK; // Return success
    }
    else
    {
      return HAL_ERROR; // Return error - Address mismatch
    }
  }
  else
  {
    return HAL_ERROR; // Return error - Failed to parse service request
  }
}

void SendDataCommand(char addr)
{
  char command[8]; // Command send data("aD0!\r\n" for e)
  // Construct the command to request measurement
  snprintf(command, sizeof(command), "%cD0!\r\n", addr);

  // Send the command to request measurement
  SDI12_SendCommand(command, sizeof(command));
  return;
}

/**
 ******************************************************************************
 * @brief    This is a initialization for the SDI-12 data line.
 *
 * @param    GPIO_TypeDef *GPIOx, an instance of the typdef struct GPIO_Typedef
 * @param    uint16_t, GPIO_Pin
 * @return   void
 ******************************************************************************
 */
void SDI12_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  sdi12.Pin = GPIO_Pin; // Configure the pin
  sdi12.Port = GPIOx;

  HAL_GPIO_WritePin(sdi12.Port, sdi12.Pin, GPIO_PIN_RESET); // Set the data line low
}

/**
 ******************************************************************************
 * @brief    Wake all sensors on the data line.
 *
 * @param    void
 * @return   void
 ******************************************************************************
 */
void SDI12_WakeSensors(void)
{
  SendContinousHigh(sdi12.Port, sdi12.Pin, WAKE_SENSOR_DELAY); // Set the data line low
  HAL_Delay(MARKING_DELAY); // Marking after break, see SDi-12 guide
}

/**
 ******************************************************************************
 * @brief    Send a command via SDI12
 *
 * @param    const char *, command
 * @return   void
 ******************************************************************************
 */
void SDI12_SendCommand(const char *command, uint8_t size)
{
  SendStartBit(sdi12.Port, sdi12.Pin); // Start bit
  // Send each character in the command
  for (uint8_t i = 0; i < size; ++i) {
    SendCharacter(sdi12.Port, sdi12.Pin, command[i]);
  }
  SendStopBit(sdi12.Port, sdi12.Pin); // Stop bit
  return;
}

/**
 ******************************************************************************
 * @brief    Read data from a sensor via SDI-12
 *
 * @param    char *, buffer
 * @param    uint16_t, bufferSize
 * @param    uint16_t, timeoutMillis
 * @param    const char *, command
 * @return   HAL_StatusTypeDef
 ******************************************************************************
 */
HAL_StatusTypeDef SDI12_ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis)
{
  uint32_t startTime = HAL_GetTick(); // Get the current time
  HAL_StatusTypeDef ret;
  // Wait for the start of the response or timeout
  while ((HAL_GetTick() - startTime) < timeoutMillis)
  {
    // Assuming data line is high during idle state
    if (HAL_GPIO_ReadPin(sdi12.Port, sdi12.Pin) == GPIO_PIN_RESET)
    {
      // Start of response detected, begin reading
      for (uint16_t index = 0; index < bufferSize - 1; ++index)
      {
        // Read a single character
        buffer[index] = ReadCharacter(sdi12.Port, sdi12.Pin);

        // Check for the end of the response
        if (index >= 2 && buffer[index - 2] == 'a' && buffer[index - 1] == '\r' && buffer[index] == '\n')
        {
          buffer[index + 1] = '\0'; // Null-terminate the buffer
          ret = HAL_OK;
          return ret;
        }
      }
    }

    // Delay for a short period before checking again
    simpleDelay(HALF_BIT_IN_MICROSECONDS);
  }

  // Handle the case where the timeout occurs
  ret = HAL_TIMEOUT;
  buffer[0] = '\0'; // Null-terminate the buffer in case of a timeout
  return ret;
}

/**
 ******************************************************************************
 * @brief    This is a function to read a measurment from a particular sensor.
 *
 * @param    char const addr, the device address
 * @param    SDI12_Measure_TypeDef, measurment data
 * @return   HAL_StatusTypeDef
 ******************************************************************************
 */
// This function will not work as is! Will need to update according to specifications to handle more complexity.
// Perhaps this is simply a start measurment command
// On a succsful parse it goes to a series of sub-functions that handle the individual cases
// Case ttt =000: Immediatly send DO command to get data
// Case ttt = some time: Wait for either of 2 things to happen either the pin goes high from send request from sensor, in which case send D0 or some time elapses
//  in which case send a wake_sensor and send a D0
HAL_StatusTypeDef SDI12_GetMeasurment(const char addr, SDI12_Measure_TypeDef *measurment_info, char *measurement_data, uint16_t timeoutMillis)
{
  char command[MAX_RESPONSE_SIZE];        // Command to request measurement ("M1!\r\n" for example)
  char responseBuffer[MAX_RESPONSE_SIZE]; // Buffer to store the response    char responseAddr; // Address in sensor response
  HAL_StatusTypeDef ret;

  SDI12_WakeSensors();

  // Construct the command to request measurement
  snprintf(command, sizeof(command), "%cM!\r\n", addr);

  // Send the command to request measurement
  SDI12_SendCommand(command, sizeof(command));

  // Read the response from the device
  ret = SDI12_ReadData(responseBuffer, sizeof(responseBuffer), timeoutMillis);
  if (ret != HAL_OK)
  {
    return ret;
  }

  // Parse the response and populate the structure
  ret = ParseMeasurementResponse(responseBuffer, addr, measurment_info);
  if (ret != HAL_OK)
  {
    return ret;
  }

  if (measurment_info->Time == 0)
  { // If data is ready now
    SendDataCommand(measurment_info->Address);
    ret = SDI12_ReadData(measurement_data, sizeof(measurement_data), timeoutMillis);
    return ret;
  }

  int millisStart = HAL_GetTick();
  while ((HAL_GetTick() - millisStart) < measurment_info->Time)
  { // Wait for ttt to elapse
    if (HAL_GPIO_ReadPin(sdi12.Port, sdi12.Pin) == GPIO_PIN_SET)
    { // If there is any activity on the data line
      ret = SDI12_ReadData(responseBuffer, sizeof(responseBuffer), timeoutMillis);
      if (ret != HAL_OK)
      {
        return ret;
      }
      ret = ParseServiceRequest(responseBuffer, measurment_info->Address);
      if (ret != HAL_OK)
      {
        return ret;
      }
      SendDataCommand(measurment_info->Address);
      ret = SDI12_ReadData(measurement_data, sizeof(measurement_data), timeoutMillis);
      return ret;
    }
  }
  SDI12_WakeSensors(); // If ttt does elapse wake the sensors
  SendDataCommand(measurment_info->Address);
  ret = SDI12_ReadData(measurement_data, sizeof(measurement_data), timeoutMillis);
  return ret;
}

/**
 ******************************************************************************
 * @brief    This is a function to ping a certain device, and see if it's active
 *
 * @param    char const addr, the device address
 * @return   HAL_StatusTypeDef
 ******************************************************************************
 */
HAL_StatusTypeDef SDI12_PingDevice(uint8_t deviceAddress, char *responseBuffer, uint16_t bufferSize, uint32_t timeoutMillis)
{
  HAL_StatusTypeDef ret;
  char command[6];          // Active Acknowledge command with address and termination character
  char expectedResponse[4]; // Expected response for Active Acknowledge
  const char *pingS = "Send a0! success\r\n";
  const char *pingUF = "Unexpected response\r\n";
  const char *pingRF = "Read data failed\r\n";

  SDI12_WakeSensors();

  // Construct the Active Acknowledge command (e.g., "a0!\r\n")
  sprintf(command, "%c!", deviceAddress);

  // Construct the expected response ("a\r\n")
  sprintf(expectedResponse, "%c\r\n", deviceAddress);
  // Send the Active Acknowledge command
  SDI12_SendCommand(command, sizeof(command));

  // Read the response from the device
  ret = SDI12_ReadData(responseBuffer, bufferSize, timeoutMillis);
  if (ret != HAL_OK)
  {
    HAL_UART_Transmit(&huart1, (const uint8_t *)pingRF, 19, 15);
    return ret;
  }
  

  // Check if the response matches the expected Active Acknowledge response
  if (strcmp(responseBuffer, expectedResponse) == 0)
  {
    ret = HAL_OK;
    HAL_UART_Transmit(&huart1, (const uint8_t *)pingS, 19, 15);
  }
  else
  {
    ret = HAL_ERROR;
    HAL_UART_Transmit(&huart1, (const uint8_t *)pingUF, 22, 15);
    HAL_UART_Transmit(&huart1, (const uint8_t *)responseBuffer, 4, 20);
  }
  return ret;
}
