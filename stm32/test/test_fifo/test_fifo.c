/**
 * @file test_fifo.c
 * @brief Tests the fifo circular buffer library
 */

#include <stdio.h>
#include <unity.h>

#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "fifo.h"


void setUp(void) {
  FramBufferClear();
}

void tearDown(void) {}

void test_FramPut_ValidData(void) {
  uint8_t data[] = {1, 2, 3, 4, 5};

  FramStatus status = FramPut(data, sizeof(data));

  TEST_ASSERT_EQUAL(FRAM_OK, status);
  TEST_ASSERT_EQUAL(1, FramBufferLen());
}

void test_FramPut_BufferFull(void) {
  uint8_t data[fram_buffer_size + 1];  // Data size is larger than the buffer size

  FramStatus status = FramPut(data, sizeof(data));

  TEST_ASSERT_EQUAL(FRAM_BUFFER_FULL, status);
}

void test_FramGet_BufferEmpty(void) {
  uint8_t data[fram_buffer_size];
  uint8_t data_len;

  FramStatus status = FramGet(data, &data_len);

  TEST_ASSERT_EQUAL(FRAM_BUFFER_EMPTY, status);
}

void test_FramGet_ValidData(void) {
  uint8_t put_data[] = {1, 2, 3, 4, 5};
  FramPut(put_data, sizeof(put_data));

  uint8_t get_data[sizeof(put_data)];
  uint8_t get_data_len;
  FramStatus status = FramGet(get_data, &get_data_len);

  TEST_ASSERT_EQUAL(FRAM_OK, status);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(put_data, get_data, sizeof(put_data));
}

void test_FramBufferLen(void) {
  // Assuming that the buffer length is initially 0
  TEST_ASSERT_EQUAL(0, FramBufferLen());

  // Put some data into the buffer
  uint8_t data[] = {1, 2, 3, 4, 5};
  FramPut(data, sizeof(data));

  // Now the buffer length should be equal to the size of the data
  TEST_ASSERT_EQUAL(1, FramBufferLen());
}

void test_FramBufferClear(void) {
  // Put some data into the buffer
  uint8_t data[] = {1, 2, 3, 4, 5};
  FramPut(data, sizeof(data));

  TEST_ASSERT_EQUAL(1, FramBufferLen());

  // Clear the buffer
  FramBufferClear();

  // Now the buffer length should be 0
  TEST_ASSERT_EQUAL(0, FramBufferLen());
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  UNITY_BEGIN();
  // other tests depend on the following two
  RUN_TEST(test_FramBufferClear);
  RUN_TEST(test_FramBufferLen);
  // test buffer functions
  RUN_TEST(test_FramPut_ValidData);
  RUN_TEST(test_FramPut_BufferFull);
  RUN_TEST(test_FramGet_ValidData);
  RUN_TEST(test_FramGet_BufferEmpty);
  UNITY_END();
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
