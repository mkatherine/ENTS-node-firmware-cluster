/**
 * @file wifi.hpp
 * @author John Madden (jmadden173@pm.me)
 * @brief Module for WiFi communication
 * @version 0.1
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULES_WIFI_H_
#define ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULES_WIFI_H_

#include "template_module.hpp"

#include "soil_power_sensor.pb.h"

class ModuleWiFi: public ModuleHandler::Module {
  public:

  ModuleWiFi(void);

  ~ModuleWiFi(void);

  /**
   * @see ModuleHandler::Module.OnReceive
   */
  void OnReceive(const Esp32Command& cmd);

  /**
   * @see ModuleHandler::Module.OnRequest
   */
  size_t OnRequest(uint8_t* buffer);

  private:

  typedef enum {
    /** Receive WiFi SSID, password, and URL */
    CONNECT = 0,
    /** Post data to the endpoint */
    POST = 1,
  } StateEnum; 
};

#endif // ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULES_WIFI_H_