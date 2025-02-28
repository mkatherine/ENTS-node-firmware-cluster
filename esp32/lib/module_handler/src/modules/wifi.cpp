#include "modules/wifi.hpp"

#include <ArduinoLog.h>

#include "http.hpp"

ModuleWiFi::ModuleWiFi(void) {
  // set module type
  type = Esp32Command_wifi_command_tag;

  // create ntp client
  timeClient = new NTPClient(ntpUDP);
}

ModuleWiFi::~ModuleWiFi(void) {
  // delete timeClient
  delete timeClient;
}

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

  case WiFiCommand_Type_CHECK:
    Log.traceln("Calling CHECK");
    Check(cmd);
    break;

  case WiFiCommand_Type_TIME:
    Log.traceln("Calling TIME");
    Time(cmd);
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
  // init return command
  WiFiCommand wifi_cmd = WiFiCommand_init_zero;
  wifi_cmd.type = WiFiCommand_Type_CONNECT;

  Log.traceln("ModuleWiFi::Connect");

  //  print the mac address
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Log.noticeln("MAC: %x:%x:%x:%x:%x:%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Log.noticeln("Connecting to WiFI...");
  Log.noticeln("ssid: %s", cmd.command.wifi_command.ssid);
  Log.noticeln("passwd: %s", cmd.command.wifi_command.passwd);

  WiFi.setHostname("esp32");

  // TODO fix
  // flag to switch between connecting and checking
  static bool conn_started = false;

  // connect to WiFi
  //WiFi.disconnect();
  int status; 

  if (!conn_started) {
    status = WiFi.begin(cmd.command.wifi_command.ssid, cmd.command.wifi_command.passwd);
    conn_started = true;
  } else {
    status = WiFi.status();
  }

  Log.noticeln("WiFi connection status: %d", status);
  
  if (status == WL_CONNECTED) {
    Log.noticeln("IP Address: %p", WiFi.localIP());
    Log.noticeln("Gateway IP: %p", WiFi.gatewayIP());
    Log.noticeln("Subnet Mask: %p", WiFi.subnetMask());
    Log.noticeln("DNS: %p", WiFi.dnsIP());

    // reconfigure DNS
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(1,1,1,1));
    
    Log.noticeln("New DNS: %p", WiFi.dnsIP());

    // start timeclient
    timeClient->begin();
  } else if (status == WL_CONNECT_FAILED) {
    Log.errorln("Connection failed!");
  } else if (status == WL_NO_SSID_AVAIL) {
    Log.errorln("SSID not available!");
  }

  // set status
  wifi_cmd.rc = status;

  request_buffer_len = EncodeWiFiCommand(&wifi_cmd, request_buffer, sizeof(request_buffer));
}

void ModuleWiFi::Post(const Esp32Command& cmd) {
  // init return WiFi command
  WiFiCommand wifi_cmd = WiFiCommand_init_zero;
  wifi_cmd.type = WiFiCommand_Type_POST;

  Log.traceln("ModuleWiFI::Post");

  // check if connected to WiFi connected
  if (WiFi.status() != WL_CONNECTED) {
    Log.errorln("Not connected to WiFi!");

    wifi_cmd.rc = 1;
  } else {
    // send measurement
    const uint8_t* meas = cmd.command.wifi_command.resp.bytes;
    const size_t meas_len = cmd.command.wifi_command.resp.size;

    // send measurement
    HttpClient resp_msg = this->dirtviz.SendMeasurement(meas, meas_len);
    uint8_t* resp = (uint8_t*) resp_msg.Data().c_str();
    size_t resp_len = resp_msg.Data().length();
    unsigned int status_code = resp_msg.ResponseCode();

    Log.noticeln("Resp length: %d", resp_len);

    // save status code and response
    wifi_cmd.rc = status_code;
    if (resp_len > 0) {
      wifi_cmd.resp.size = resp_len;
      memcpy(wifi_cmd.resp.bytes, resp, resp_len);
    }
  }

  // encode wifi command in buffer
  this->request_buffer_len = EncodeWiFiCommand(
    &wifi_cmd,
    request_buffer,
    sizeof(request_buffer)
  );
}

void ModuleWiFi::Check(const Esp32Command& cmd) {
  Log.traceln("ModuleWiFi::Check");

  WiFiCommand wifi_cmd = WiFiCommand_init_zero;
  wifi_cmd.type = WiFiCommand_Type_CHECK;

  // check if connected to WiFi connected
  uint8_t wifi_status = WiFi.status();
  if (wifi_status != WL_CONNECTED) {
    Log.errorln("Not connected to WiFi!");

    wifi_cmd.rc = wifi_status;
  } else {
    // set url
    dirtviz.SetUrl(cmd.command.wifi_command.url);
    dirtviz.SetPort(cmd.command.wifi_command.port);

    // TODO Add API status check
    Log.notice("Checking API endpoint...\t");
    wifi_cmd.rc = dirtviz.Check();
    Log.noticeln("%d", wifi_cmd.rc);
  }

  request_buffer_len = EncodeWiFiCommand(
    &wifi_cmd,
    request_buffer,
    sizeof(request_buffer)
  );
}

void ModuleWiFi::Time(const Esp32Command& cmd) {
  Log.traceln("ModuleWiFi::Time");

  WiFiCommand wifi_cmd = WiFiCommand_init_zero;
  wifi_cmd.type = WiFiCommand_Type_TIME;

  // update NTP server url
  /*
  if (cmd.command.wifi_command.url != NULL) {
    Log.traceln("Upating pool name to: %s", cmd.command.wifi_command.url);
    timeClient->setPoolServerName(cmd.command.wifi_command.url);
  }
  */
  
  // check if connected to WiFi connected
  uint8_t wifi_status = WiFi.status();
  if (wifi_status != WL_CONNECTED) {
    Log.errorln("Not connected to WiFi!");

    wifi_cmd.ts = 0;
  } else {
    // force update
    if (timeClient->update()) {
      wifi_cmd.ts = timeClient->getEpochTime();
      Log.traceln("Current timestamp: %d", wifi_cmd.ts);
    } else {
      Log.traceln("Failed to get time from NTP server!");
      wifi_cmd.ts = 0;
    }
  }

  request_buffer_len = EncodeWiFiCommand(
    &wifi_cmd,
    request_buffer,
    sizeof(request_buffer)
  );
}
