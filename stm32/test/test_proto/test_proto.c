/**
 * @file
 * @brief 
 * 
 * [Extended description here]
 * 
 * @see [header file]
 * 
 * @author
 * @date [Date created]
*/

#include "main.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>

#include <unity.h>

#include "pb_encode.h"
#include "pb_decode.h"

#include "timestamp.pb.h"
#include "soil_power_sensor.pb.h"



#ifndef UNIX_EPOCHS
/** Unix epochs encoded in timestamp. Default corresponds to Mon Nov 27 2023
 * 03:25:19 GMT+0000 */
#define UNIX_EPOCHS 1701055519
#endif /* UNIX_EPOCHS */

#ifndef FLOAT_DELTA
/** Delta between comparison between floating point numbers */
#define FLOAT_DELTA 1e-9
#endif /* FLOAT_DELTA */


void SystemClock_Config(void);
  
/** Global variable for all return codes */
HAL_StatusTypeDef rc;




/**
 * @brief Setup code that runs at the start of every test
 * 
 * Creates required variables that are shared between all tests. The output
 * buffer is initialized based on length of the buffer. Input buffer is defined,
 * but not initialized since it depends on the message length. Default values
 * are defined for Timestamp and MeasurementMetadata.
*/
void setUp(void)
{
  // buffer to store encoded data
  uint8_t buffer[256];
  // message length
  size_t msg_len;

  // output buffer
  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  // input buffer
  pb_istream_t istream;

  // status code for nanopb
  bool status;

  // Default timestamp
  google_protobuf_Timestamp ts_default;
  ts_default.seconds = UNIX_EPOCHS;
  ts_default.nanos = 0;

  // Default measurement metadata
  MeasurementMetadata meta_default;
  meta_default.has_ts = true;
  meta_default.ts = ts_default;
  meta_default.cell_id = 0;
  meta_default.logger_id = 0;
}

/**
 * @brief Tear down code that runs at the end of every test
*/
void tearDown(void) {}


void test_timestamp(void)
{
  // Create message corresponding to Mon Nov 27 2023 03:25:19 GMT+0000
  google_protobuf_Timestamp ts_encode = google_protobuf_Timestamp_init_zero;
  ts_encode.seconds = UNIX_EPOCHS;
  ts_encode.nanos = 0;

  // encode 
  status = pb_encode(&ostream, google_protobuf_Timestamp_fields, &ts_encode);
  msg_len = ostream.bytes_written;
  
  TEST_ASSERT_FALSE(status);
  TEST_ASSERT_GREATER_THAN(0, msg_len);


  google_protobuf_Timestamp ts_decode = google_protobuf_Timestamp_init_zero;

  // decode
  istream = pb_istream_from_buffer(buffer, msg_len);
  status = pb_decode(&istream, google_protobuf_Timestamp_fields, &ts_decode);

  TEST_ASSERT_FALSE(status);
  TEST_ASSERT_EQUAL(UNIX_EPOCHS, ts_decode.seconds);
  TEST_ASSERT_EQUAL(0, ts_decode.nanos);
}

void test_meta(void)
{
  MeasurementMetadata meta_encode = google_protobuf_Timestamp_init_zero;

  meta_encode.has_ts = true;
  meta_encode.ts = ts_default;
  meta_encode.cell_id = 0;
  meta_encode.logger_id = 1;

  status = pb_encode(&ostream, MeasurementMetadata_fields, &meta_encode);
  msg_len = ostream.bytes_written;

  TEST_ASSERT_FALSE(status);
  TEST_ASSERT_GREATER_THAN(0, msg_len);


  MeasurementMetadata meta_decode;

  istream = pb_istream_from_buffer(buffer, msg_len);
  status = pb_decode(&istream, MeasurementMetadata_fields, &meta_decode);

  TEST_ASSERT_FALSE(status);

  TEST_ASSERT_EQUAL(0, meta_decode.cell_id);
  TEST_ASSERT_EQUAL(1, meta_decode.logger_id);

  TEST_ASSERT_TRUE(meta_decode.has_ts);
  TEST_ASSERT_EQUAL(UNIX_EPOCHS, meta_decode.ts.seconds);
  TEST_ASSERT_EQUAL(0, meta_decode.ts.nanos); 

}

void test_power(void)
{
  PowerMeasurement power_encode = PowerMeasurement_init_zero;
  power_encode.has_meta = true;
  power_encode.meta = meta_default;
  power_encode.voltage = 1.2;
  power_encode.current = 120.1;

  status = pb_encode(&ostream, PowerMeasurement_fields, &power_encode);
  msg_len = status.bytes_written;

  TEST_ASSERT_FALSE(status);
  TEST_ASSERT_GREATER_THAN(0, msg_len);


  PowerMeasurement power_decode;

  istream = pb_istream_from_buffer(buffer, msg_len);
  status = pb_decode(&istream, PowerMeasurement_fields, &power_decode);

  TEST_ASSERT_FALSE(status);

  // Meta
  TEST_ASSERT_TRUE(power_decode.has_meta);
  TEST_ASSERT_EQUAL(0, power_decode.meta.cell_id);
  TEST_ASSERT_EQUAL(0, power_decode.meta.logger_id);
  // Timestamp
  TEST_ASSERT_TRUE(power_decode.has_meta);
  TEST_ASSERT_EQUAL(UNIX_EPOCHS, power_decode.meta.ts.seconds);
  TEST_ASSERT_EQUAL(0, power_decode.meta.ts.nanos);

  TEST_ASSERT_DOUBLE_WITHIN(FLOAT_DELTA, 1.2, power_decode.voltage);
  TEST_ASSERT_DOUBLE_WITHIN(FLOAT_DELTA, 120.1, power_decode.current);
}

void test_teros12(void)
{
  Teros12Measurement teros12_encode = Teros12Measurement_init_zero;
  teros12_encode.has_meta = true;
  teros12_encode.meta = meta_default;
  teros12_encode.ec = 200;
  teros12_encode.temp = 24.5;
  teros12_encode.vwc_adj = 32.4;
  teros12_encode.vwc_raw = 3200.2;

  status = pb_encode(&ostream, Teros12Measurement_fields, &teros12_encode);
  msg_len = status.bytes_written;


  Teros12Measurement teros12_decode;

  istream = pb_istream_from_buffer(buffer, msg_len);
  status = pb_decode(&istream, Teros12Measurement_fields, &teros12_decode);

  TEST_ASSERT_FALSE(status);

  // Meta
  TEST_ASSERT_TRUE(teros12_decode.has_meta);
  TEST_ASSERT_EQUAL(0, teros12_decode.meta.cell_id);
  TEST_ASSERT_EQUAL(0, teros12_decode.meta.logger_id);
  // Timestamp
  TEST_ASSERT_TRUE(teros12_decode.has_meta);
  TEST_ASSERT_EQUAL(UNIX_EPOCHS, teros12_decode.meta.ts.seconds);
  TEST_ASSERT_EQUAL(0, teros12_decode.meta.ts.nanos);

  TEST_ASSERT_EQUAL(200, teros12_decode.ec);
  TEST_ASSERT_DOUBLE_WITHIN(FLOAT_DELTA, 24.5, teros12_decode.temp);
  TEST_ASSERT_DOUBLE_WITHIN(FLOAT_DELTA, 32.4, teros12_decode.vwc_adj);
  TEST_ASSERT_DOUBLE_WITHIN(FLOAT_DELTA, 3200.2, teros12_decode.vwc_raw);
}


/**
  * @brief Entry point for battery test
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

  // Unit testing
  UNITY_BEGIN();

  // Tests for timestamp
  RUN_TEST(test_timestamp);
  // Tests for MeasurementMetadata
  RUN_TEST(test_meta);
  // Tests for PowerMeasurement
  RUN_TEST(test_power);
  // Tests for Teros12
  RUN_TEST(test_teros12);

  UNITY_END();
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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);


  /* USER CODE BEGIN Error_Handler_Debug */
  char error[30];
  int error_len = sprintf(error, "Error!  HAL Status: %d\n", rc);
  HAL_UART_Transmit(&huart1, error, error_len, 1000);

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
