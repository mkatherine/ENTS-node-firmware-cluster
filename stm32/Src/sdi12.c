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

 #include "sdi12.h"

 SDI12_TypeDef sdi12;

/**
  ******************************************************************************
  * @brief    This is a initialization for the SDI-12 data line.
  * 
  * @param    GPIO_TypeDef *GPIOx, an instance of the typdef struct GPIO_Typedef
  * @param    uint16_t, GPIO_Pin
  * @return   HAL_StatusTypeDef
  ******************************************************************************
  */
 void SDI_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    sdi12.Pin = GPIO_Pin; // Configure the pin
    sdi12.Port = GPIOx;

    HAL_GPIO_WritePin(sdi12.Port, sdi12.Pin, GPIO_PIN_RESET); // Set the data line low
 }

 // Example of how to use the functions
void SDI12_SendCommand(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, const char *command) {
    SDI12_SendStartBit(GPIOx, GPIO_Pin);  // Start bit

    // Send each character in the command
    for (const char *ptr = command; *ptr != '\0'; ++ptr) {
        SDI12_SendCharacter(GPIOx, GPIO_Pin, *ptr);
    }

    SDI12_SendStopBit(GPIOx, GPIO_Pin);  // Stop bit
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
 HAL_StatusTypeDef SDI12_GetMeasurment(const char addr, const SDI12_Measure_TypeDef *measurment_info){
   wake_Sensors(sdi12.Port, sdi12.Pin); // Wake all the sensors
    
 }

