/**
 * @file transcoder.c
 * 
 * @see transcoder.h
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-01-04
*/

#include "transcoder.h"

#include "usart.h"

/**
 * @brief Encodes a measurement
 * 
 * Serializes a generic measurement using protobuf and stores result in buffer.
 * The resulting number of bytes is returned with -1 indicating there was an
 * error.
 * 
 * @param meas Measurement
 * @param buffer Buffer to store serialized measurement
 * @return Length of buffer, -1 indicates there was an error
*/
size_t EncodeMeasurement(Measurement *meas, uint8_t *buffer);

size_t EncodePowerMeasurement(int64_t ts, uint32_t logger_id,
                              uint32_t cell_id, double voltage,
                              double current, uint8_t *buffer)
{
  Measurement meas = Measurement_init_zero;

  meas.has_meta = true;

  meas.meta.has_ts = true;
  meas.meta.ts.seconds = ts;
  meas.meta.ts.nanos = 0;

  meas.meta.logger_id = logger_id;
  meas.meta.cell_id = cell_id;

  meas.which_measurement = Measurement_power_tag;
  meas.measurement.power.voltage = voltage;
  meas.measurement.power.current = current;
  
  return EncodeMeasurement(&meas, buffer);
}


size_t EncodeTeros12Measurement(int64_t ts, uint32_t logger_id,
                                uint32_t cell_id, float vwc_raw, float vwc_adj,
                                float temp, uint32_t ec, uint8_t *buffer)
{
  Measurement meas = Measurement_init_zero;

  meas.has_meta = true;

  meas.meta.has_ts = true;
  meas.meta.ts.seconds = ts;
  meas.meta.ts.nanos = 0;

  meas.meta.logger_id = logger_id;
  meas.meta.cell_id = cell_id;

  meas.which_measurement = Measurement_teros12_tag;
  meas.measurement.teros12.vwc_raw = vwc_raw;
  meas.measurement.teros12.vwc_adj = vwc_adj;
  meas.measurement.teros12.temp = temp;
  meas.measurement.teros12.ec = ec;

  return EncodeMeasurement(&meas, buffer);
}

Response_ResponseType DecodeResponse(const uint8_t *data, const size_t len)
{
  Response resp;

  // create input buffer
  pb_istream_t istream = pb_istream_from_buffer(data, len);
  // decode data and check status
  bool status = pb_decode(&istream, Response_fields, &resp);
  if (!status)
  {
    return -1;
  }

  // return response type
  return resp.resp;
}

size_t EncodeMeasurement(Measurement *meas, uint8_t *buffer)
{
  // create output stream
  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, 256);
  // encode message and check rc
  bool status = pb_encode(&ostream, Measurement_fields, meas);
  if (!status)
  {
    return -1;
  }

  // return number of bytes written
  return ostream.bytes_written;
}