/**
 * @file bme280_sensor.h
 * @author John Madden (jmadden173@pm.me)
 * @brief Sensor interface for the BME280 sensor
 * @version 0.1
 * @date 2024-05-17
 * 
 * @copyright Copyright (c) 2024
 * 
 * This is the user code file that allows for interaction with the BME280
 * sensor. It provides a function for reading each measurement and a function
 * for a function that can be integrated with the sensors api.
 * 
 * @see sensors.h
 */

#ifndef __BME280_H_
#define __BME280_H_

#include <stdint.h>
#include <stddef.h>

#include "i2c.h"
#include "stm32wlxx_hal_i2c.h"
#include "stm32wlxx_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  BME280_OK,
  BME280_ERROR
} BME280Status;

BME280Status BME280Init(void);

BME280Status BME280Deinit(void);

BME280Status BME280MeasureTemperature(void);

BME280Status BME280MeasurePressure(void);

BME280Status BME280MeasureHumidity(void);

/**
 * @brief BME280 sensor library function
 * 
 * All three measurements (temperature, pressure, humidity) are measured and
 * appropriate calibration are applied. Data gets encoded into a serialized
 * measurement.
 * 
 * @param data Buffer to store measurement
 * @return Length of measurement
 * 
 * @see SensorsPrototypeMeasure
 */
size_t BME280Measure(uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif /* __BME280_H */