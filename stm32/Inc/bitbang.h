/**
  ******************************************************************************
  * @file     bitbang.h
  * @author   Stephen Taylor
  * @brief    This file contains all the gpio implementation of SDI12
  * 
  * @date     2/2/2024
  ******************************************************************************
  */

#ifndef __BITBANG_H__
#define __BITBANG_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "i2c.h"
#include "usart.h"

#include <stdio.h>
#include <stdlib.h>

#define WAKE_SENSOR_DELAY 12

void wake_Sensors(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void SDI12_SendStartBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void SDI12_SendStopBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void SDI12_SendCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char character);


#ifdef __cplusplus
}
#endif

#endif /* __BITBANG_H__ */