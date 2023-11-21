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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "fram.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  //uint32_t value_adc;
  char output[100];
  char output1[42];
  const unsigned char * test_intro = "\r\nThis is the FRAM library test harness\r\n";
  const unsigned char * HAL_intto = "Testing HAL libraries\r\n";
  const unsigned char * FRAM_failure = "FRAM did not respond\r\n";
  const unsigned char * mem_failure = "Failed to read/write to mem_address 0\r\n";
  const unsigned char * success = "Success\r\n";
  const unsigned char * failure = "Failure\r\n";
  const unsigned char * Basic_intro = "Testing basic read/write functions\r\n";
  uint8_t test = 34;
  uint8_t recieved;
  uint8_t data[] = {25, 50, 75, 100};
  uint8_t rec[4];
  uint8_t data1[] = {3, 4, 9, 100, 10, 50, 25, 23, 25, 25, 7, 9, 10, 11, 12};
  uint8_t rec1[10];
  uint8_t rec2[5];

  HAL_UART_Transmit(&huart1, test_intro, 42, 1000);

  /**
  ******************************************************************************
  HAL Library Tests
  ******************************************************************************
  **/
  HAL_UART_Transmit(&huart1, HAL_intto, 24, 1000);

  if (HAL_I2C_Mem_Write(&hi2c2, FM24_WRITE, 0x00, I2C_MEMADD_SIZE_8BIT, &test, 1, 50) == HAL_OK){
    HAL_UART_Transmit(&huart1, success, 10, 1000);
  } else {
    HAL_UART_Transmit(&huart1, failure, 10, 1000);
    HAL_UART_Transmit(&huart1, FRAM_failure, 22, 1000);
  } 

  if (HAL_I2C_Mem_Read(&hi2c2, FM24_READ, 0x00, I2C_MEMADD_SIZE_8BIT, &recieved, 1, 50) == HAL_OK){
    if (recieved == test){
      HAL_UART_Transmit(&huart1, success, 10, 1000);
    } else {
      HAL_UART_Transmit(&huart1, failure, 10, 1000);
      HAL_UART_Transmit(&huart1, mem_failure, 22, 1000);
    }
  } else {
    HAL_UART_Transmit(&huart1, failure, 10, 1000);
    HAL_UART_Transmit(&huart1, FRAM_failure, 22, 1000);
  }
  /**
  ******************************************************************************
  FRAM Library Basic Read/Write
  ******************************************************************************
  **/
  HAL_UART_Transmit(&huart1, Basic_intro, 37, 1000);

  FRAM_Write(data, 4);

  if (FRAM_Read(rec, 4) == HAL_OK){
    if ((data[3] == rec[0]) && (data[2] == rec[1]) && (data[1] == rec[2]) && (data[0] == rec[3])){
      HAL_UART_Transmit(&huart1, success, 10, 1000);
    } else {
      HAL_UART_Transmit(&huart1, failure, 10, 1000);
    }
  }
  sprintf(output, "Wrote: %d,%d,%d,%d\r\n", data[0],data[1],data[2],data[3]);
  HAL_UART_Transmit(&huart1, output, 21, 10);
  sprintf(output, "Read: %d,%d,%d,%d\r\n", rec[0],rec[1],rec[2],rec[3]);
  HAL_UART_Transmit(&huart1, output, 20, 10);

  FRAM_Write(data1, 15);
  if (FRAM_Read(rec1, 10) == HAL_OK){
    if ((data1[14] == rec1[0])&& 
    (data1[13] == rec1[1]) && 
    (data1[12] == rec1[2]) && 
    (data1[11] == rec1[3]) &&
    (data1[10] == rec1[4]) &&
    (data1[9] == rec1[5]) &&
    (data1[8] == rec1[6]) &&
    (data1[7] == rec1[7]) &&
    (data1[6] == rec1[8]) &&
    (data1[5] == rec1[9])){
      HAL_UART_Transmit(&huart1, success, 10, 1000);
    } else {
      HAL_UART_Transmit(&huart1, failure, 10, 1000);
    }
  }

  FRAM_Read(rec2, 5);
  reverseArray(rec2, rec2[0], rec2[4]);
  if ((data1[0] == rec2[0])&& 
    (data1[1] == rec2[1]) && 
    (data1[2] == rec2[2]) && 
    (data1[3] == rec2[3]) &&
    (data1[4] == rec2[4])){
     HAL_UART_Transmit(&huart1, success, 10, 1000);
    } else {
      HAL_UART_Transmit(&huart1, failure, 10, 1000);
    }

  sprintf(output1, "Wrote: %d,%d,%d,%d,%d\r\n", data1[0],data1[1],data1[2],data1[3],data1[4]);
  HAL_UART_Transmit(&huart1, output1, 24, 10);
  sprintf(output, "Read: %d,%d,%d,%d,%d\r\n", rec2[0],rec2[1],rec2[2],rec2[3],rec2[4]);
  HAL_UART_Transmit(&huart1, output, 23, 10);
  





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



    //HAL_UART_Transmit(&huart1, buffer, len, 1000);

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_Delay(500);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
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
