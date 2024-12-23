#include "controller/wifi.h"

#include "transcoder.h"
#include "communication.h"

/** Timeout for i2c communication with esp32 */
unsigned int g_controller_i2c_timeout = 10000;

uint32_t ControllerWiFiInit(
  const char* ssid,
  const char* passwd,
  const char* url,
  const uint32_t port
) {
  // get reference to tx and rx buffers
  Buffer* tx = ControllerTx();
  Buffer* rx = ControllerRx();

  // encode command
  tx->len = EncodeWiFiCommand(WiFiCommand_Type_CONNECT, ssid, passwd, url, port,
                             0, 0, NULL, 0, tx->data, tx->size);

  // send transaction
  ControllerStatus status = CONTROLLER_SUCCESS;
  status = ControllerTransaction(g_controller_i2c_timeout);
  if (status != CONTROLLER_SUCCESS) {
    return -1;
  }

  // check for errors
  if (rx->len == 0) {
    return -1;
  }

  // decode command
  Esp32Command cmd = Esp32Command_init_default;
  cmd = DecodeEsp32Command(rx->data, rx->len);

  // return timestamp
  return cmd.command.wifi_command.ts;
}

int ControllerWiFiPost(
  const uint8_t* data,
  size_t data_len,
  uint8_t* resp,
  uint8_t* resp_len
) {
  // return code
  int rc = 0;

  // get reference to tx and rx buffers
  Buffer* tx = ControllerTx();
  Buffer* rx = ControllerRx();

  // encode command
  tx->len = EncodeWiFiCommand(WiFiCommand_Type_POST, NULL, NULL, NULL, 0, 0, 0,
                             data, data_len, tx->data, tx->size);

  // return if communication fails
  ControllerStatus status = CONTROLLER_SUCCESS;
  status = ControllerTransaction(g_controller_i2c_timeout);
  if (status != CONTROLLER_SUCCESS) {
    return 0;
  }
 

  // check for errors
  if (rx->len == 0) {
    return 0;
  }
  
  // decode command
  Esp32Command cmd = Esp32Command_init_default;
  cmd = DecodeEsp32Command(rx->data, rx->len);
  rc = cmd.command.wifi_command.rc;

  // copy response if not empty
  *resp_len = cmd.command.wifi_command.resp.size;
  if (*resp_len > 0) {
    memcpy(resp, cmd.command.wifi_command.resp.bytes, cmd.command.wifi_command.resp.size);
  }

  // return http code
  return rc;
}
