/**
 * @file transcoder.h
 * 
 * @brief Implements encoding of various types of measurements and decoding
 * responses
 * 
 * If more functionality is required, use the protobuf generated code from
 * soil_power_sensor.h.
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2024-01-04
*/

#ifndef __TRANSCODER_H__
#define __TRANSCODER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "pb_encode.h"
#include "pb_decode.h"

#include "timestamp.pb.h"
#include "soil_power_sensor.pb.h"

/**
 * @brief Encodes a power measurement
 * 
 * @param ts Unix epochs
 * @param logger_id Logger Id
 * @param cell_id Cell Id
 * @param voltage Voltage measured in mV
 * @param current Current measured in uA
 * @param buffer Buffer to store serialized measurement
 * @return Number of bytes in @p buffer
*/
size_t EncodePowerMeasurement(int64_t ts, uint32_t logger_id,
                              uint32_t cell_id, double voltage,
                              double current, uint8_t *buffer);

/**
 * @brief Encodes a power measurement
 * 
 * @param ts Timestamp
 * @param logger_id Logger Id
 * @param cell_id Cell Id
 * @param vwc_raw Raw volumetric water content
 * @param vwc_adj Volumetric water content converted to percentage
 * @param temp Temperature in celsius
 * @param ec Electrical conductivity
 * @param buffer Buffer to store serialized measurement
 * @return Number of bytes in @p buffer
*/
size_t EncodeTeros12Measurement(int64_t ts, uint32_t logger_id,
                                uint32_t cell_id, float vwc_raw, float vwc_adj,
                                float temp, uint32_t ec, uint8_t *buffer);

/**
 * @brief Decodes a response message
 * 
 * @param data Serialized data
 * @param len Number of bytes in @p data
 * @return Response type
*/
Response_ResponseType DecodeResponse(uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __TRANSCODER_H__ */