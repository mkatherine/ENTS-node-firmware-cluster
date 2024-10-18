/**
 ******************************************************************************
 * @file    sdi12.c
 * @author  Stephen Taylor
 * @brief   This file provides a library to communicate between a STM32 MC
 *          and a TEROS-12 via SDI-12.
 *          https://www.sdi-12.org/
 * 
 * @date    4/1/2024
 *
 ******************************************************************************
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdi12.h"

static const uint16_t REQUEST_MEASURMENT_RESPONSE_SIZE = 7;
static const uint16_t SERVICE_REQUEST_SIZE = 3;
static const uint16_t MEASURMENT_DATA_SIZE = 18;
static const uint16_t SEND_COMMAND_TIMEOUT = 1000;

/* Helper function to parse the sensor's response to a get measurment command*/
SDI12Status ParseMeasurementResponse(const char *responseBuffer, char addr, SDI12_Measure_TypeDef *measurement_info);

/* Helper function to parse a sensor's service request */
SDI12Status ParseServiceRequest(const char *requestBuffer, char addr);

void SDI12WakeSensors(void){
    HAL_LIN_SendBreak(&huart2); // Send a break 
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); // Send the marking 
    //HAL_Delay(20); // Need an extra 10ms to account for the fact that HAL_LIN_SendBreak is nonblocking
    for (int i = 0; i <= 40000; i++){ // Figure out a way to do this delay with the low-power timer
      asm("nop");
    };
}

SDI12Status SDI12SendCommand(const char *command, uint8_t size){
    HAL_StatusTypeDef ret;
    SDI12WakeSensors();
    ret = HAL_UART_Transmit(&huart2, (const uint8_t *) command, size, SEND_COMMAND_TIMEOUT);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // Set to RX mode
    if (ret == HAL_OK) {
      return SDI12_OK;
    } else {
      return SDI12_ERROR;
    }
}

SDI12Status SDI12ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis){
  HAL_StatusTypeDef ret;
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // Set to RX mode
  __HAL_UART_FLUSH_DRREGISTER(&huart2);
  ret = HAL_UART_Receive(&huart2,(uint8_t *) buffer, bufferSize, timeoutMillis);
  if (ret == HAL_OK){
    return SDI12_OK;
  } else if (ret == HAL_TIMEOUT) {
    return SDI12_TIMEOUT_ON_READ;
  } else {
    return SDI12_ERROR;
  }
}

SDI12Status ParseMeasurementResponse(const char *responseBuffer, char addr, SDI12_Measure_TypeDef *measurement_info)
{
  sscanf(responseBuffer, "%1c%3hu%1hhu", &(measurement_info->Address), &(measurement_info->Time), &(measurement_info->NumValues)); // Parse the response and populate the structure
  // Check if the response address matches the expected address
  if (measurement_info->Address == addr)
  {
    return SDI12_OK; // Return success
  }
  else
  {
    return SDI12_PARSING_ERROR; // Return error - Address mismatch
  }
}

SDI12Status ParseServiceRequest(const char *requestBuffer, char addr)
{
  char expectedResponse[12];
  sprintf(expectedResponse, "%c\r\n", addr); // Construct the expected response ("a\r\n")
  // Parse the service request
  if (memcmp(requestBuffer, expectedResponse, 3) == 0) // Check if the response matches the expected Active Acknowledge response
  {
    return SDI12_OK;
  }
  else
  {
    return SDI12_PARSING_ERROR;
  }
}

SDI12Status SDI12GetMeasurment(uint8_t addr, SDI12_Measure_TypeDef *measurment_info, char *measurment_data, uint16_t timeoutMillis)
{
  char reqMeas[30];        // Command to request measurement ("0!\r\n" for example)
  char sendData[30];       // Command to send the data
  SDI12Status ret;

  uint8_t size = sprintf(reqMeas, "%cM!", addr); // Construct a command to request a measurment

  char buffer[7];
  SDI12SendCommand(reqMeas, size); // Request a measurment and read the response
  ret = SDI12ReadData(buffer, REQUEST_MEASURMENT_RESPONSE_SIZE, timeoutMillis);
  if (ret != SDI12_OK){
    return ret;
  }
    

  size = sprintf(sendData, "%cD0!", addr); // Construct a command to send the data
  ret = ParseMeasurementResponse(buffer, addr, measurment_info); // Check if the addresses match from the response above, the response from a teros is the same every time so we're going to leave it for now
  if (ret != SDI12_OK){
    return ret;
  }

  if (measurment_info->Time == 0){ // If data is ready now
    SDI12SendCommand(sendData, size); 
    ret = SDI12ReadData(measurment_data, MEASURMENT_DATA_SIZE, timeoutMillis); // hard coded for a teros measurment response size
    return ret;
  }

  ret = SDI12ReadData(buffer, SERVICE_REQUEST_SIZE, timeoutMillis); // Read the service request, wait's for one to arrive
  if (ret != SDI12_OK)
  {
    return ret;
  }
  ret = ParseServiceRequest(buffer, measurment_info->Address); // make sure the service request is correct
  if (ret != SDI12_OK)
  {
    return ret;
  }
  SDI12SendCommand(sendData, size);
  ret = SDI12ReadData(measurment_data, MEASURMENT_DATA_SIZE, timeoutMillis); // IT IS VERY IMPORTANT THAT THE AMOUNT OF DATA EXPECTED TO BE READ (bufferSize) MATCH WHAT IS SENT EXACTLY. Or maybe slightly smaller, but any greater causes the program to hang
  return ret;
}
