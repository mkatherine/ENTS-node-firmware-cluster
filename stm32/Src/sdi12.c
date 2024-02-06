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

 #include "sdi12.h"

 SDI12_TypeDef sdi12;

/**
  ******************************************************************************
  * @brief    This is a initialization for the SDI-12 data line.
  * 
  * @param    GPIO_TypeDef *GPIOx, an instance of the typdef struct GPIO_Typedef
  * @param    uint16_t, GPIO_Pin
  * @return   void
  ******************************************************************************
  */
 void SDI12_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
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
 void SDI12_WakeSensors(void){
    SendContinousHigh(sdi12.Port, sdi12.Pin, WAKE_SENSOR_DELAY); // Set the data line low
 }

/**
  ******************************************************************************
  * @brief    Send a command via SDI12
  * 
  * @param    const char *, command
  * @return   void
  ******************************************************************************
  */
void SDI12_SendCommand(const char *command) {
    SendStartBit(sdi12.Port, sdi12.Pin);  // Start bit

    // Send each character in the command
    while (*command != '\0') {
        SendCharacter(sdi12.Port, sdi12.Pin, *command);
    }

    SendStopBit(sdi12.Port, sdi12.Pin);  // Stop bit
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
HAL_StatusTypeDef SDI12_ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis){
  uint32_t startTime = HAL_GetTick();  // Get the current time
  HAL_StatusTypeDef ret;
  // Wait for the start of the response or timeout
    while ((HAL_GetTick() - startTime) < timeoutMillis) {
        // Assuming data line is high during idle state
        if (HAL_GPIO_ReadPin(sdi12.Port, sdi12.Pin) == GPIO_PIN_RESET) {
            // Start of response detected, begin reading
            for (uint16_t index = 0; index < bufferSize - 1; ++index) {
                // Read a single character
                buffer[index] = ReadCharacter(sdi12.Port, sdi12.Pin);

                // Check for the end of the response
                if (index >= 2 && buffer[index - 2] == 'a' && buffer[index - 1] == '\r' && buffer[index] == '\n') {
                    buffer[index + 1] = '\0';  // Null-terminate the buffer
                    ret = HAL_OK;
                    return ret;
                }
            }
        }

        // Delay for a short period before checking again
        delayMicroseconds(HALF_BIT_IN_MICROSECONDS);
    }

    // Handle the case where the timeout occurs
    ret = HAL_TIMEOUT;
    buffer[0] = '\0';  // Null-terminate the buffer in case of a timeout
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
 //This function will not work as is! Will need to update according to specifications to handle more complexity.
 //Perhaps this is simply a start measurment command
 //On a succsful parse it goes to a series of sub-functions that handle the individual cases
 //Case ttt =000: Immediatly send DO command to get data
 //Case ttt = some time: Wait for either of 2 things to happen either the pin goes high from send request from sensor, in which case send D0 or some time elapses
 // in which case send a wake_sensor and send a D0
 HAL_StatusTypeDef SDI12_GetMeasurment(const char addr, const SDI12_Measure_TypeDef *measurment_info, uint16_t timeoutMillis){
    char command[6];  // Command to request measurement ("M1!\r\n" for example)
    char responseBuffer[MAX_RESPONSE_SIZE];  // Buffer to store the response

    // Construct the command to request measurement
    snprintf(command, sizeof(command), "M%c!\r\n", addr);

    // Send the command to request measurement
    SDI12_SendCommandToDevice(sdi12.Port, sdi12.Pin, command);

    // Read the response from the device
    SDI12_ReadData(responseBuffer, sizeof(responseBuffer), timeoutMillis);

    // Parse the response and populate the measurement_info structure
    // You need to implement this part based on the actual response format from your sensor
    // For now, let's assume a simple response format: "M123,456,789\r\n" where 123, 456, 789 are values

    // Parse the response and populate the structure
    if (sscanf(responseBuffer, "M%u,%u,%u", &(measurement_info->Value1), &(measurement_info->Value2), &(measurement_info->Value3)) == 3) {
        // Successfully parsed the response, update other fields
        measurement_info->Address = addr;  // Update address
        measurement_info->NumValues = 3;   // Update the number of values
        // You might need to update Time field based on your actual response format

        return HAL_OK;  // Return success
    } else {
        // Failed to parse the response
        return HAL_ERROR;  // Return error
    }
  
 }

/**
  ******************************************************************************
  * @brief    This is a function to ping a certain device, and see if it's active
  * 
  * @param    char const addr, the device address
  * @return   HAL_StatusTypeDef
  ******************************************************************************
  */
 HAL_StatusTypeDef SDI12_PingDevice(uint8_t deviceAddress, char *responseBuffer, uint16_t bufferSize, uint32_t timeoutMillis){
  HAL_StatusTypeDef ret;
  char command[5];  // Active Acknowledge command with address and termination character
  char expectedResponse[4];  // Expected response for Active Acknowledge

  // Construct the Active Acknowledge command (e.g., "a0!\r\n")
  sprintf(command, sizeof(command), "a%c!\r\n", deviceAddress);

  // Construct the expected response ("a\r\n")
  sprintf(expectedResponse, sizeof(expectedResponse), "a\r\n");

  // Send the Active Acknowledge command
  SDI12_SendCommand(command);

  // Read the response from the device
  ret = SDI12_ReadData(responseBuffer, bufferSize, timeoutMillis);
  if (ret != HAL_OK){
    return ret;
  }

    // Check if the response matches the expected Active Acknowledge response
    if (strcmp(responseBuffer, expectedResponse) == 0) {
        ret = HAL_OK;
    } else {
        ret = HAL_ERROR;
    }
  return ret;
}
  
 
