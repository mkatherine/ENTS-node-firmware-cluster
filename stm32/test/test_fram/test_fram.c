// Copyright 2023 UCSC

/**
 * @file test_fram.c
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

void setUp(void) {}

/**
 * @brief Tear down code that runs at the end of every test
*/
void tearDown(void) {}

void test_i2c(void) {
  uint8_t test = 185;
  uint8_t received;

  HAL_StatusTypeDef status;

  // test raw i2c write
  status = HAL_I2C_Mem_Write(&hi2c2, 0xA0, 0x00, I2C_MEMADD_SIZE_8BIT, &test,
                             1, 50);
  TEST_ASSERT_EQUAL(HAL_OK, status);

  // test raw i2c read
  status = HAL_I2C_Mem_Read(&hi2c2, 0xA1, 0x00, I2C_MEMADD_SIZE_8BIT, &received,
                            1, 50);
  TEST_ASSERT_EQUAL(HAL_OK, status);
  TEST_ASSERT_EQUAL(test, received);
}

void test_FramWrite_ValidData(void) {
  uint8_t data[] = {1, 2, 3, 4, 5};
  uint16_t addr = 0x00;

  FramStatus status = FramWrite(addr, data, sizeof(data));

  TEST_ASSERT_EQUAL(FRAM_OK, status);
}

void test_FramWrite_OutOfRange(void) {
  uint8_t data[] = {1, 2, 3, 4, 5};
  uint16_t addr = fram_max_addr + 1;  // Address out of range

  FramStatus status = FramWrite(addr, data, sizeof(data));

  TEST_ASSERT_EQUAL(FRAM_OUT_OF_RANGE, status);
}

void test_FramWrite_ZeroLength(void) {
  uint8_t data[] = {1, 2, 3, 4, 5};
  uint16_t addr = 0x00;

  FramStatus status = FramWrite(addr, data, 0);

  TEST_ASSERT_EQUAL(FRAM_OK, status);
}

void test_FramWrite_MultiplePages(void) {
  uint8_t data[] = {1, 2, 3, 4, 5};
  // right below the segment size
  uint16_t addr = FRAM_SEG_SIZE-1;

  FramStatus status = FramWrite(addr, data, sizeof(data));

  TEST_ASSERT_EQUAL(FRAM_OK, status);
}

void test_FramRead_Status(void) {
  uint8_t data[5];
  uint16_t addr = 0x0;

  FramStatus status = FramRead(addr, sizeof(data), data);

  TEST_ASSERT_EQUAL(FRAM_OK, status);
}

void test_FramRead_ValidData(void) {
  uint16_t addr = 0x00;

  uint8_t write_data[] = {1, 2, 3, 4, 5};
  FramWrite(addr, write_data, sizeof(write_data));

  uint8_t read_data[5];
  FramStatus status = FramRead(addr, sizeof(read_data), read_data);

  TEST_ASSERT_EQUAL(FRAM_OK, status);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(write_data, read_data, 5);
}

void test_FramRead_ZeroLength(void) {
  uint8_t data[5];
  uint16_t addr = 0x0;

  FramStatus status = FramRead(addr, 0, data);

  TEST_ASSERT_EQUAL(FRAM_OK, status);
}

void test_FramRead_OutOfRange(void) {
  uint8_t data[5];
  uint16_t addr = fram_max_addr + 1;

  FramStatus status = FramRead(addr, sizeof(data), data);

  TEST_ASSERT_EQUAL(FRAM_OUT_OF_RANGE, status);
}

void test_FramRead_MultiplePages(void) {
  // right below the segment size
  uint16_t addr = FRAM_SEG_SIZE-1;

  uint8_t write_data[] = {1, 2, 3, 4, 5};
  FramWrite(addr, write_data, sizeof(write_data));

  uint8_t read_data[5];
  FramStatus status = FramRead(addr, sizeof(read_data), read_data);

  TEST_ASSERT_EQUAL(FRAM_OK, status);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(write_data, read_data, 5);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();

  // wait for UART
  for (int i = 0; i < 1000000; i++) {
      __NOP();
  }

  UNITY_BEGIN();
  RUN_TEST(test_i2c);
  RUN_TEST(test_FramWrite_ValidData);
  RUN_TEST(test_FramWrite_ZeroLength);
  RUN_TEST(test_FramWrite_OutOfRange);
  RUN_TEST(test_FramWrite_MultiplePages);
  RUN_TEST(test_FramRead_ValidData);
  RUN_TEST(test_FramRead_ZeroLength);
  RUN_TEST(test_FramRead_OutOfRange);
  RUN_TEST(test_FramRead_MultiplePages);
  UNITY_END();
}
