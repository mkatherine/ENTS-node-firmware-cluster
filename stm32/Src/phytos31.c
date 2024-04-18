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

HAL_StatusTypeDef Phytos31Init(){
  return ADS12_init();
}

phytos_measurments Phytos31GetMeasurment(){
  phytos_measurments measurments;
  
}
