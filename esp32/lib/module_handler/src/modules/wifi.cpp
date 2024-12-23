#include "modules/wifi.hpp"

#include <ArduinoLog.h>

#include "http.hpp"

ModuleWiFi::ModuleWiFi(void) {
  // set module type
  type = Esp32Command_wifi_command_tag;
}

ModuleWiFi::~ModuleWiFi(void) {}

void ModuleWiFi::OnReceive(const Esp32Command& cmd) {
  Log.traceln("ModuleWiFi::OnReceive");

  // check if WiFi command
  if (cmd.which_command != Esp32Command_wifi_command_tag) {
    return;
  }

  Log.traceln("WiFiCommand type: %d", cmd.command.wifi_command.type);

  // switch for command types
  switch (cmd.command.wifi_command.type)
  {
  case WiFiCommand_Type_CONNECT:
    Log.traceln("Calling CONNECT");
    Connect(cmd);
    break;

  case WiFiCommand_Type_POST:
    Log.traceln("Calling POST");
    Post(cmd);
    break;
  
  default:
    Log.warningln("wifi command type not found!");
    break;
  }
}

size_t ModuleWiFi::OnRequest(uint8_t* buffer) {
  Log.traceln("ModuleWiFi::OnRequest");
  memcpy(buffer, request_buffer, request_buffer_len);
  return request_buffer_len;
}

void ModuleWiFi::Connect(const Esp32Command& cmd) {
  // timestamp
  uint32_t ts = -1;

  Log.traceln("ModuleWiFi::Connect");

  //  print the mac address
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Log.noticeln("MAC: %x:%x:%x:%x:%x:%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Log.noticeln("Connecting to WiFI...");
  Log.noticeln("ssid: %s", cmd.command.wifi_command.ssid);
  Log.noticeln("passwd: %s", cmd.command.wifi_command.passwd);

  // connect to WiFi
  // early return if SSID is not available
  int status = WiFi.begin(cmd.command.wifi_command.ssid, cmd.command.wifi_command.passwd);
  Log.noticeln("WiFi connection status: %d", status);
  

  // flag if connected for future code
  bool connected = false;

  // wait for WiFi to connect
  while (1) {
    // TODO implement lp sleep
    delay(1000);
    Log.noticeln("Waiting for connection...");

    status = WiFi.status();
    Log.noticeln("wifi.begin: %d", status);
    if (status == WL_CONNECTED) {
      Log.noticeln("Connected with ip: %p", WiFi.localIP());
      connected = true;
      break;
    } else if (status == WL_CONNECT_FAILED) {
      Log.errorln("Connection failed!");
      break;
    } else if (status == WL_NO_SSID_AVAIL) {
      Log.errorln("SSID not available!");
      break;
    }
  } 

  if (connected) {
    // set url
    dirtviz.SetUrl(cmd.command.wifi_command.url);
    dirtviz.SetPort(cmd.command.wifi_command.port);

    // TODO Add API status check
    Log.noticeln("Checking API endpoint");
    ts = dirtviz.Check();
    Log.noticeln("Current timestamp is %d", ts);
  }

  request_buffer_len = EncodeWiFiCommand(
    WiFiCommand_Type_CONNECT,
    "",
    "",
    "",
    0,
    status,
    ts,
    nullptr,
    0,
    request_buffer,
    sizeof(request_buffer)
  );
}

void ModuleWiFi::Post(const Esp32Command& cmd) {
  Log.traceln("ModuleWiFI::Post");

  // check if connected to WiFi connected
  if (WiFi.status() != WL_CONNECTED) {
    Log.errorln("Not connected to WiFi!");

    // encode with return code 1 to indicate WiFi connection error
    this->request_buffer_len = EncodeWiFiCommand(
      WiFiCommand_Type_POST,
      nullptr,
      nullptr,
      nullptr,
      0,
      1,
      0,
      nullptr,
      0,
      request_buffer,
      sizeof(request_buffer)
    );

    return;
  }

  // send measurement
  const uint8_t* meas = cmd.command.wifi_command.resp.bytes;
  const size_t meas_len = cmd.command.wifi_command.resp.size;

  // send measurement
  HttpClient resp_msg = this->dirtviz.SendMeasurement(meas, meas_len);
  uint8_t* resp = (uint8_t*) resp_msg.Data().c_str();
  size_t resp_len = resp_msg.Data().length();
  unsigned int status_code = resp_msg.ResponseCode();

  Log.noticeln("Resp length: %d", resp_len);

  // encode wifi command in buffer
  this->request_buffer_len = EncodeWiFiCommand(
    WiFiCommand_Type_POST,
    nullptr,
    nullptr,
    nullptr,
    0,
    status_code,
    0,
    resp,
    resp_len,
    request_buffer,
    sizeof(request_buffer)
  );
}
