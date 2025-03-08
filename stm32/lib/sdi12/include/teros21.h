/**
 * @file teros21.h
 * @author John Madden <jmadden173@pm.me>
 * @brief Drives for reading measurements from Teros21 sensor
 * @date 2025-03-05
 */

#ifndef LIB_SDI12_INCLUDE_TEROS21_H_
#define LIB_SDI12_INCLUDE_TEROS21_H_

#include <stdint.h>

#include "sdi12.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char addr;
  float matric_pot;
  float temp;
} Teros21Data;

/**
 * @brief Parse measurement string from Teros21 sensor
 */
SDI12Status Teros21ParseMeasurement(const char *buffer, Teros21Data *data);

/**
 * @brief Read and parse a Teros21 measurement
 *
 * @param addr Address of the sensor
 * @param data Pointer to the data structure to store the measurement
 * @return SDI12Status
 */
SDI12Status Teros21GetMeasurement(char addr, Teros21Data *data);

/**
 * @brief Get measurement from Teros21 sensor
 *
 * Measures a Teros21 at address 0 and encodes it into a serialized
 * measurement.
 *
 * @param data Buffer to store measurement
 * @return Length of measurement
 *
 * @see SensorsPrototypeMeasure
 *
 */
size_t Teros21Measure(uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif  // LIB_SDI12_INCLUDE_TEROS21_H_
