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
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>

#include "usart.h"
#include "gpio.h"
#include "lptim.h"
#include "user_config.h"

#define WAKE_SENSOR_DELAY 13
#define MARKING_DELAY 9
#define MAX_RESPONSE_SIZE 100
#define ACTIVE_AWCKNOWLEDGE_COMMAND_SIZE 2
#define START_MEASUREMENT_COMMAND_SIZE 3
#define START_MEASURMENT_RESPONSE_SIZE 7
#define SEND_DATA_COMMAND_SIZE 4
#define SEND_DATA_RESPONSE_SIZE 16 // You need to add the numvalues (n) value returned from start measurment
#define SECONDS_TO_MILLISECONDS 1000
#define SERVICE_REQUEST_SIZE 3
#define SCALE_VWC_TO_INT 100
#define SCALE_TMP_TO_INT 10

typedef struct
{
  char Address;
  uint16_t Time;
  uint8_t NumValues;
} SDI12_Measure_TypeDef;

typedef struct
{
  int ec;
  float vwc_raw;
  float vwc_adj;
  float tmp;
  int addr;
} Teros12_Data;

void LPTIM_Delay_Start();

void LPTIM_Delay(uint32_t delay_ms);
/**
******************************************************************************
* @brief    Wake all sensors on the data line.
*
* @param    void
* @return   void
******************************************************************************
*/
void SDI12_WakeSensors(void);

/**
******************************************************************************
* @brief    Send a command via SDI12
*
* @param    const char *, command
* @return   void
******************************************************************************
*/
HAL_StatusTypeDef SDI12_SendCommand(const char *command, uint8_t size);

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
HAL_StatusTypeDef SDI12_ReadData(char *buffer, uint16_t bufferSize, uint16_t timeoutMillis);

#ifdef __cplusplus
}
#endif

#endif /* __SDI12_H__ */