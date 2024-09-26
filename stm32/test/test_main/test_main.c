/**
 * @file test_main.c
 * @brief Tests the fram library
 */

#include <stdio.h>
#include <unity.h>

#include "main.h"
#include "main_helper.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "fram.h"

void setUp(void) {

}

/**
 * @brief Tear down code that runs at the end of every test
*/
void tearDown(void) {}

void test_GetTick(void){
  uint32_t ticks = HAL_GetTick();
  TEST_ASSERT_NOT_EQUAL(ticks, 0); // if zero, it means timer was never started
}



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_LoRaWAN_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  ADC_init();
  MX_RTC_Init();

  // wait for UART
  for (int i = 0; i < 1000000; i++) {
      __NOP();
  }

  UNITY_BEGIN();
  RUN_TEST(test_GetTick);
  UNITY_END();
}
