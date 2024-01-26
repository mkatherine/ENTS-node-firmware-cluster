/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file     ads.h
  * @author   Stephen Taylor
  * @brief    This file contains all the function prototypes for
  *           the ads.c file
  * @date     11/27/2023
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "usart.h"

#include <stdio.h>


/* USER CODE END Includes */
/* USER CODE BEGIN PD */
#define ADS12_WRITE 0x80
#define ADS12_READ 0x81

#define ADS12_START_CODE 0x08
#define ADS12_RESET_CODE 0x06
#define ADS12_POWERDOWN_CODE 0x02
#define ADS12_READ_DATA_CODE 0x10
#define ADS12_WRITE_TO_REGISTER 0x40E2
#define ADS12_READ_CONTROL_REGISTER 0x20
#define ADS12_CONTROL_REGISTER 0b00000001

#define TWO_POW_TWENTYTHREE 8388608 // Eqn 5 in ADS1219 datasheet
/* USER CODE END PD */

/* USER CODE BEGIN 1 */

HAL_StatusTypeDef ADC_init(void);
int ADC_read(void);
HAL_StatusTypeDef probeADS12(void);


/* USER CODE END 1 */