/**
 ******************************************************************************
 * @file     phytos31.h
 * @author   Stephen Taylor
 * @brief    This file contains all the function prototypes for
 *           the phytos31.c file.
 * 
 *           This library is designed to read measurements from a PHYTOS-31 
 *           sensor from METER.
 *           https://metergroup.com/products/phytos-31/
 * @date     4/18/2024
 ******************************************************************************
 */

#ifndef __PHYTOS31_H__
#define __PHYTOS31_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include "ads.h"

typedef struct {
    double phytos31_raw;
    double phytos31_calibrated;
} phytos_measurments;

/**
******************************************************************************
* @brief    Wrapper function for the ADC initilization. 
*
* @param    void
* @return   HAL_StatusTypeDef
******************************************************************************
*/
HAL_StatusTypeDef Phytos31Init(void);

/**
******************************************************************************
* @brief    Returns both the raw voltage value and a calibrated measurment
*           from a PHYTOS31 sensor.
*
* @param    void
* @return   phytos_measurments
******************************************************************************
*/
phytos_measurments Phytos31GetMeasurment(void);



#ifdef __cplusplus
}
#endif

#endif /* __PHYTOS31_H__ */