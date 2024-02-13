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

#define ONE_BIT_IN_MICROSECONDS 833 // Rate of 1 bit of data transfer at 1200 baud
#define HALF_BIT_IN_MICROSECONDS 416 // ~Half the rate of 1 bit of data transfer at 1200 baud
#define HZ_TO_MHZ 1000000 // Scale Hz to MHz
#define FOUR_MHZ 4
#define ONE_COMMAND_IN_MICROSECONDS_AT_FOURTY_EIGHT_MHZ 6.8 // Holy magic numbers, this is used in the simple delay function to scale commands to timing


void delayMicroseconds(uint32_t microseconds);
void simpleDelay(uint16_t microseconds);
void SendContinousHigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t milliseconds);
void SendStartBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void SendStopBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void SendCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char character);
char ReadCharacter(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#ifdef __cplusplus
}
#endif

#endif /* __BITBANG_H__ */