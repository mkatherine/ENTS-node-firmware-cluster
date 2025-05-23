/**
 * @file wifi.hpp
 * @author John Madden (jmadden173@pm.me)
 * @brief Module for WiFi communication
 * @version 0.1
 * @date 2024-08-23
 *
 * The WiFi module support CONNECT and POST commands through OnReceive. The
 * CONNECT command connects to a WiFi network and returns the timestamp from the
 * server for time syncronization purposes. The POST requires sends a HTTP POST
 * to the configured hub URL and returns the data from the HTTP response.
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LIB_MODULE_HANDLER_INCLUDE_MODULES_WIFI_HPP_
#define LIB_MODULE_HANDLER_INCLUDE_MODULES_WIFI_HPP_

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "dirtviz.hpp"
#include "soil_power_sensor.pb.h"
#include "template_module.hpp"
#include "transcoder.h"

class ModuleWiFi : public ModuleHandler::Module {
 public:
  ModuleWiFi(void);

  ~ModuleWiFi(void);

  /**
   * @see ModuleHandler::Module.OnReceive
   */
  void OnReceive(const Esp32Command &cmd);

  /**
   * @see ModuleHandler::Module.OnRequest
   */
  size_t OnRequest(uint8_t *buffer);

 private:
  typedef enum {
    /** Receive WiFi SSID, password, and URL */
    CONNECT = 0,
    /** Post data to the endpoint */
    POST = 1,
  } StateEnum;

  /**
   * @brief Object interface to Dirtviz
   *
   */
  Dirtviz dirtviz;

  void Post(const Esp32Command &cmd);

  void Connect(const Esp32Command &cmd);

  void Check(const Esp32Command &cmd);

  void Time(const Esp32Command &cmd);

  WiFiUDP ntpUDP;

  NTPClient *timeClient;

  /** Buffer for i2c requests */
  uint8_t request_buffer[WiFiCommand_size] = {};
  size_t request_buffer_len = 0;
};

#endif  // LIB_MODULE_HANDLER_INCLUDE_MODULES_WIFI_HPP_
