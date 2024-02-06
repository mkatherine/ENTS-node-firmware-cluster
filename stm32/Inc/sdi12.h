/**
  ******************************************************************************
  * @file     sdi12.h
  * @author   Stephen Taylor
  * @brief    This file contains all the driver functions for the sdi12 interface
  * 
  * @date     1/30/2024
  ******************************************************************************
  */

#ifndef __SDI12_H__
#define __SDI12_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "i2c.h"
#include "usart.h"
#include "bitbang.h"

#include <stdio.h>
#include <stdlib.h>

#define WAKE_SENSOR_DELAY 12000
#define MAX_RESPONSE_SIZE 10

typedef struct {
    uint32_t Pin;
    GPIO_TypeDef *Port;
} SDI12_TypeDef;

typedef struct {
    char Address;
    uint16_t Time;
    uint8_t NumValues;
} SDI12_Measure_TypeDef;

void SDI12_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void SDI12_WakeSensors(void);
void SDI12_SendCommand(const char *command);
HAL_StatusTypeDef SDI12_ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis);
HAL_StatusTypeDef SDI12_GetMeasurment(const char addr, const SDI12_Measure_TypeDef *measurment_info, uint16_t timeoutMillis);
HAL_StatusTypeDef SDI12_PingDevice(uint8_t deviceAddress, char *responseBuffer, uint16_t bufferSize, uint32_t timeoutMillis);


#ifdef __cplusplus
}
#endif

#endif /* __SDI12_H__ */