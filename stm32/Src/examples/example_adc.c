/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "fram.h"
#include "ads.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FM24_WRITE 0xA0 // Device address of FM24 in write mode
#define FM24_READ 0xA1 // Device address of FM24 in read mode


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
 /**
  ******************************************************************************
  * @brief    Helper function to reverse array
  * @param    arr array
  * @param    start start of array
  * @param    end end of array
  * @return   none
  ******************************************************************************
  */
void reverseArray(int arr[], int start, int end) 
{ 
    int temp; 
    while (start < end) { 
        temp = arr[start]; 
        arr[start] = arr[end]; 
        arr[end] = temp; 
        start++; 
        end--; 
    } 
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
  
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  ADC_init();
  /* USER CODE BEGIN 2 */

  // Print the compilation time at startup
  char info_str[100];
  int info_len;
  info_len = sprintf(
    info_str,
    "Soil Power Sensor Wio-E5 firmware, compiled on %s %s\n",
    __DATE__,__TIME__
    );
  HAL_UART_Transmit(&huart1, (const uint8_t *) info_str, info_len, 1000);


  uint32_t battery_voltage = 0;


  // Calibrate and start conversion process
  rc = HAL_ADCEx_Calibration_Start(&hadc);
  if (rc != HAL_OK) Error_Handler();

  rc = HAL_ADC_Start_DMA(&hadc, (uint32_t *) &battery_voltage, 1);
  if (rc != HAL_OK) Error_Handler();

  /* USER CODE BEGIN 2 */
  ADC_init();
  //uint8_t ADC_Reading[3];
  // const unsigned char * intro = "Hello\r\n";
  // const unsigned char * fail = "I2C Failed\r\n";
  // const unsigned char * succsuful_probe = "Probe succseeded\r\n";
  char output[20];
  // if(probeADS12() != HAL_OK){
  //   HAL_UART_Transmit(&huart1, fail, 13, 19);
  // } else {
  //   HAL_UART_Transmit(&huart1, succsuful_probe, 19, 9);
  // };

  int reading;
  int count = 0;
  // sprintf(output, "Read: %x,%x,%x\r\n", ADC_Reading[0],ADC_Reading[1],ADC_Reading[2]);
  // HAL_UART_Transmit(&huart1, output, 17, 19);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int adc_val;
  char adc_out[20];
  while (1)
  {

    /* USER CODE END WHILE */
    // reading = ADC_read();
    

    /* USER CODE BEGIN 3 */
    reading = ADC_read();
    sprintf(output, "Adjusted: %d\r\n\r\n", reading);
    HAL_UART_Transmit(&huart1, (const uint8_t *) output, 17, 19);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_Delay(1000);
    count++;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  char error[30];
  int error_len = sprintf(error, "Error!  HAL Status: %d\n", rc);
  HAL_UART_Transmit(&huart1, (const uint8_t *) error, error_len, 1000);

  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
