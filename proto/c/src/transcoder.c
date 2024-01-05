/**
 * @file transcoder.c
 * 
 * @see transcoder.h
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-01-04
*/

#include "transcoder.h"

size_t EncodePowerMeasurement(uint16_t ts, uint32_t logger_id,
                              uint32_t cell_id, double voltage,
                              double current, uint8_t *buffer)
{
  return 0;
}


size_t EncodeTeros12Measurement(uint16_t ts, uint32_t logger_id,
                                uint32_t cell_id, float vwc_raw, float vwc_adj,
                                float temp, uint32_t ec, uint8_t *buffer)
{
  return 0;
}

Response_ResponseType DecodeResponse(uint8_t *data, size_t len)
{
  return Response_ResponseType_SUCCESS;
}