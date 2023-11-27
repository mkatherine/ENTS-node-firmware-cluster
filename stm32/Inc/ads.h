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

#define ADS12_STARTUP_CODE 0x8
#define ADS12_RESET_CODE 0x6
#define ADS12_POWERDOWN_CODE 0x2
/* USER CODE END PD */

/* USER CODE BEGIN 1 */

void ADC_init(void);
int ADC_read(void);


/* USER CODE END 1 */