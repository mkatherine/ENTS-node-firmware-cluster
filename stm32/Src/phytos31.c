/**
 ******************************************************************************
 * @file    sdi12.c
 * @author  Stephen Taylor
 * 
 * @brief   This library is designed to read measurements from a PHYTOS-31 
 *          sensor from METER.
 *          https://metergroup.com/products/phytos-31/
 * @date    4/18/2024
 ******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phytos31.h"
#include "transcoder.h"

HAL_StatusTypeDef Phytos31Init(){
  return ADC_init();
}

phytos_measurments Phytos31GetMeasurment(){
  phytos_measurments measurments;
  measurments.phytos31_raw = (double) ADC_readVoltage();
  return measurments;
}

size_t Phytos31_measure(uint8_t *data) {
  // get timestamp
  time_t ts = TimestampNow();
  phytos_measurments measurment;

  // read voltage
  measurment = Phytos31GetMeasurment();
  double adc_voltage_float = ((double) measurment.phytos31_raw) / 1000.;

  // encode measurement
  size_t data_len = EncodePhytos31Measurement((uint32_t) ts, LOGGER_ID, CELL_ID,
                                           adc_voltage_float, 0.0, data);

  // return number of bytes in serialized measurement 
  return data_len;
}
