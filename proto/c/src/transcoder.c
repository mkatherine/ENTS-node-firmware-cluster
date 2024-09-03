/**
 * @file transcoder.c
 * 
 * @see transcoder.h
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-01-04
*/


#include "pb_encode.h"
#include "pb_decode.h"
#include "transcoder.h"


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

/**
 * @brief Encodes a esp32command
 * 
 * Serializes a esp32command object and stores result in buffer. The resulting
 * number of bytes is returned with -1 indicating there was an error.
 * 
 * @param cmd Reference to command
 * @param buffer Buffer to store serialized esp32command
 * @param size Size of buffer
 * 
 * @return Length of buffer, -1 indicates there was an error
 */
size_t EncodeEsp32Command(const Esp32Command *cmd, uint8_t *buffer, size_t size);

size_t EncodePowerMeasurement(uint32_t ts, uint32_t logger_id,
                              uint32_t cell_id, double voltage,
                              double current, uint8_t *buffer)
{
  Measurement meas = Measurement_init_zero;

  meas.has_meta = true;

  meas.meta.ts = ts;
  meas.meta.logger_id = logger_id;
  meas.meta.cell_id = cell_id;

  meas.which_measurement = Measurement_power_tag;
  meas.measurement.power.voltage = voltage;
  meas.measurement.power.current = current;
  
  return EncodeMeasurement(&meas, buffer);
}


size_t EncodeTeros12Measurement(uint32_t ts, uint32_t logger_id,
                                uint32_t cell_id, double vwc_raw,
                                double vwc_adj, double temp, uint32_t ec,
                                uint8_t *buffer)
{
  Measurement meas = Measurement_init_zero;

  meas.has_meta = true;

  meas.meta.ts = ts;
  meas.meta.logger_id = logger_id;
  meas.meta.cell_id = cell_id;

  meas.which_measurement = Measurement_teros12_tag;
  meas.measurement.teros12.vwc_raw = vwc_raw;
  meas.measurement.teros12.vwc_adj = vwc_adj;
  meas.measurement.teros12.temp = temp;
  meas.measurement.teros12.ec = ec;

  return EncodeMeasurement(&meas, buffer);
}

size_t EncodePhytos31Measurement(uint32_t ts, uint32_t logger_id,
                                 uint32_t cell_id, double voltage,
                                 double leaf_wetness, uint8_t *buffer)
{
  Measurement meas = Measurement_init_zero;

  meas.has_meta = true;

  meas.meta.ts = ts;
  meas.meta.logger_id = logger_id;
  meas.meta.cell_id = cell_id;

  meas.which_measurement = Measurement_phytos31_tag;
  meas.measurement.phytos31.voltage = voltage;
  meas.measurement.phytos31.leaf_wetness = leaf_wetness;

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

Esp32Command DecodeEsp32Command(const uint8_t *data, const size_t len)
{
  Esp32Command cmd;

  pb_istream_t istream = pb_istream_from_buffer(data, len);
  pb_decode(&istream, Esp32Command_fields, &cmd);

  return cmd;
}

size_t EncodePageCommand(PageCommand_RequestType req, int fd, size_t bs,
                         size_t n, uint8_t* buffer, size_t size)
{
  // create command object
  Esp32Command cmd = Esp32Command_init_default;
  cmd.which_command = Esp32Command_page_command_tag;
  cmd.command.page_command.file_request = req;
  cmd.command.page_command.file_descriptor = fd;
  cmd.command.page_command.block_size = bs;
  cmd.command.page_command.num_bytes = n;

  return EncodeEsp32Command(&cmd, buffer, size);
}

size_t EncodeTestCommand(TestCommand_ChangeState state, int32_t data,
                         uint8_t *buffer, size_t size)
{
  Esp32Command cmd = Esp32Command_init_default;
  cmd.which_command = Esp32Command_test_command_tag;
  cmd.command.test_command.state = state;
  cmd.command.test_command.data = data;

  return EncodeEsp32Command(&cmd, buffer, size);
}

size_t EncodeWiFiCommand(
  WiFiCommand_Type type,
  const char* ssid,
  const char* passwd,
  const char* url,
  uint32_t port,
  uint32_t rc,
  uint32_t ts,
  const uint8_t* resp,
  size_t resp_len,
  uint8_t* buffer,
  size_t size
) {
  Esp32Command cmd = Esp32Command_init_default;

  cmd.wifi_command.type = type;
  strcpy(cmd.wifi_command.ssid, ssid);
  strcpy(cmd.wifi_command.passwd, passwd);
  strcpy(cmd.wifi_command.url, url);
  cmd.wifi_command.port = port;
  cmd.wifi_command.rc = rc;
  cmd.wifi_command.ts = ts;
  memcpy(cmd.wifi_command.resp.bytes, resp, resp_len);
  cmd.wifi_command.resp.size = resp_len;

  return EncodeEsp32Command(&cmd, buffer, size);
}

size_t EncodeEsp32Command(const Esp32Command *cmd, uint8_t *buffer, size_t size)
{
  // create output stream
  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, size);
  // encode message and check rc
  bool status = pb_encode(&ostream, Measurement_fields, cmd);
  if (!status)
  {
    return -1;
  }

  // return number of bytes written
  return ostream.bytes_written;
}