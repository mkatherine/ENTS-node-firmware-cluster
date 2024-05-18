/**
 * Copyright (C) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "sys_app.h"
#include "stm32wlxx_hal.h"
#include "i2c.h"

#include "bme280.h"
#include "bme280_common.h"

/******************************************************************************/
/*!                               Macros                                      */

#define BME280_SHUTTLE_ID  UINT8_C(0x33)

/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr;

/**
 * @brief Timeout for I2C calls
 */
static const uint32_t i2c_timeout = 10;

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * I2C read function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    dev_addr = *(uint8_t*)intf_ptr;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, dev_addr << 1, reg_addr,
                                                I2C_MEMADD_SIZE_8BIT, reg_data,
                                                length, i2c_timeout);

    if (status != HAL_OK) {
        return BME280_E_COMM_FAIL;
    }

    return BME280_OK
}

/*!
 * I2C write function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    dev_addr = *(uint8_t*)intf_ptr;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, dev_addr << 1,
                                                 reg_addr, I2C_MEMADD_SIZE_8BIT,
                                                 reg_data, length, i2c_timeout);

    if (status != HAL_OK) {
        return BME280_E_COMM_FAIL;
    }

    return BME280_OK;
}

/*!
 * SPI read function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    // not implemented
    return 0;
}

/*!
 * SPI write function map to COINES platform
 */
BME280_INTF_RET_TYPE bme280_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    // not implemented
    return 0;
}

/*!
 * Delay function map to COINES platform
 */
void bme280_delay_us(uint32_t period, void *intf_ptr)
{
    // calculate us to ms
    uint32_t period_ms = period / 1000;

    // if no period then set to 1 to ensure delay
    if (period_ms == 0) {
        period_ms = 1;
    }

    // delay
    HAL_Delay(period_ms);
}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void bme280_error_codes_print_result(const char api_name[], int8_t rslt)
{
    if (rslt != BME280_OK)
    {
        APP_LOG(TS_ON, VLEVEL_M, "%s\t", api_name);

        switch (rslt)
        {
            case BME280_E_NULL_PTR:
                APP_LOG(TS_ON, VLEVEL_M, "Error [%d] : Null pointer error.", rslt);
                APP_LOG(TS_ON, VLEVEL_M, 
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case BME280_E_COMM_FAIL:
                APP_LOG(TS_ON, VLEVEL_M, "Error [%d] : Communication failure error.", rslt);
                APP_LOG(TS_ON, VLEVEL_M, 
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case BME280_E_DEV_NOT_FOUND:
                APP_LOG(TS_ON, VLEVEL_M, "Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case BME280_E_INVALID_LEN:
                APP_LOG(TS_ON, VLEVEL_M, "Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            default:
                APP_LOG(TS_ON, VLEVEL_M, "Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}

/*!
 *  @brief Function to select the interface between SPI and I2C.
 */
int8_t bme280_interface_selection(struct bme280_dev *dev, uint8_t intf)
{
    int8_t rslt = BME280_OK;
    struct coines_board_info board_info;

    if (dev != NULL)
    {
        int16_t result = coines_open_comm_intf(COINES_COMM_INTF_USB, NULL);

        if (result < COINES_SUCCESS)
        {
            APP_LOG(TS_ON, VLEVEL_M, 
                "\n Unable to connect with Application Board ! \n" " 1. Check if the board is connected and powered on. \n" " 2. Check if Application Board USB driver is installed. \n"
                " 3. Check if board is in use by another application. (Insufficient permissions to access USB) \n");
            exit(result);
        }

        result = coines_get_board_info(&board_info);

#if defined(PC)
        setbuf(stdout, NULL);
#endif

        if (COINES_SUCCESS != result)
        {
            APP_LOG(TS_ON, VLEVEL_M, "\n Unable to retrieve board information ! \n");
            exit(COINES_E_FAILURE);
        }

        if ((board_info.shuttle_id != BME280_SHUTTLE_ID))
        {
            APP_LOG(TS_ON, VLEVEL_M, "! Warning invalid sensor shuttle \n ," "This application will not support this sensor \n");
            exit(COINES_E_FAILURE);
        }

        coines_set_shuttleboard_vdd_vddio_config(0, 0);
        coines_delay_msec(100);

        /* Bus configuration : I2C */
        if (intf == BME280_I2C_INTF)
        {
            APP_LOG(TS_ON, VLEVEL_M, "I2C Interface\n");

            dev_addr = BME280_I2C_ADDR_PRIM;
            dev->read = bme280_i2c_read;
            dev->write = bme280_i2c_write;
            dev->intf = BME280_I2C_INTF;
			/* SDO pin is made low*/
            coines_set_pin_config(COINES_SHUTTLE_PIN_SDO, COINES_PIN_DIRECTION_OUT, COINES_PIN_VALUE_LOW);

            /* set the sensor interface as I2C with 100kHz speed */
            result = coines_config_i2c_bus(COINES_I2C_BUS_0, COINES_I2C_STANDARD_MODE);
        }
        /* Bus configuration : SPI */
        else if (intf == BME280_SPI_INTF)
        {
            APP_LOG(TS_ON, VLEVEL_M, "SPI Interface\n");

            dev_addr = COINES_SHUTTLE_PIN_7;
            dev->read = bme280_spi_read;
            dev->write = bme280_spi_write;
            dev->intf = BME280_SPI_INTF;

            result = coines_config_spi_bus(COINES_SPI_BUS_0, COINES_SPI_SPEED_5_MHZ, COINES_SPI_MODE3);
        }

        if(result != COINES_SUCCESS)
        {
            rslt = COINES_E_COMM_INIT_FAILED;
        }

        /* Holds the I2C device addr or SPI chip selection */
        dev->intf_ptr = &dev_addr;

        /* Configure delay in microseconds */
        dev->delay_us = bme280_delay_us;

        coines_delay_msec(100);

        coines_set_shuttleboard_vdd_vddio_config(3300, 3300);

        coines_delay_msec(100);
    }
    else
    {
        rslt = BME280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief Function deinitializes coines platform.
 */
void bme280_coines_deinit(void)
{
    coines_close_comm_intf(COINES_COMM_INTF_USB, NULL);
}