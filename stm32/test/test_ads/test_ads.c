/**
 * @file test_ads.c
 * @brief Tests the ads library
 */

#include <stdio.h>
#include <unity.h>

#include "main.h"
#include "main_helper.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "sys_app.h"

#include "fifo.h"
#include "ads.h"


void setUp(void) {}

void tearDown(void) {}

void test_ADC_Init(void) {
  HAL_StatusTypeDef ret;
  ret = ADC_init();

  TEST_ASSERT_EQUAL(HAL_OK, ret);
}

void test_ADC_readVoltage(void) {
  // Data size is larger than the buffer size
  double volt = ADC_readVoltage();

  TEST_ASSERT_NOT_EQUAL(volt, -1);
}

void test_ADC_readCurrent(void) {
  // Data size is larger than the buffer size
  double curr = ADC_readCurrent();

  TEST_ASSERT_NOT_EQUAL(curr, -1);
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
  UTIL_TIMER_Init();

  // wait for UART
  for (int i = 0; i < 1000000; i++) {
      __NOP();
  }

  UNITY_BEGIN();
  // other tests depend on the following two
  RUN_TEST(test_ADC_Init);
  RUN_TEST(test_ADC_readVoltage);
  RUN_TEST(test_ADC_readCurrent);
  UNITY_END();
  /* USER CODE END 3 */
}
