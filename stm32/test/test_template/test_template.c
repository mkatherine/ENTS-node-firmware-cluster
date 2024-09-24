/**
 * @file test_template.c
 * @brief Template file for tests
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2024-01-05
*/

#include "main.h"
#include "main_helper.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>

#include <unity.h>

void SystemClock_Config(void);
  

/**
 * @brief Setup code that runs at the start of every test
*/
void setUp(void) {}

/**
 * @brief Tear down code that runs at the end of every test
*/
void tearDown(void) {}


void test_true(void)
{
  TEST_ASSERT_TRUE(1);
}

/**
  * @brief Entry point for protobuf test
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  // wait for UART
  for (int i = 0; i < 1000000; i++) {
      __NOP();
  }

  // Unit testing
  UNITY_BEGIN();

  // Tests for timestamp
  RUN_TEST(test_true);

  UNITY_END();
}
