/**
 * @file test_main.cpp
 * @author John Madden (jmadden173@pm.me)
 * @brief Test WiFi code
 * @version 0.1
 * @date 2024-10-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <unity.h>

#include "module_handler.hpp"
#include "modules/wifi.hpp"

// create handler
static ModuleHandler::ModuleHandler module_handler;

static ModuleWiFi module_wifi;

void setUp(void) {
  // zero out buffers
  memset(module_handler_rx_buffer, 0, module_handler_buffer_size);
  memset(module_handler_tx_buffer, 0, module_handler_buffer_size);
}

void tearDown(void) {}

void test_RegisterModule(void) {
  module_handler.RegisterModule(Esp32Command_wifi_command_tag, &module_wifi);

  bool mod_exist = true;

  // check if module exists in map
  try {
    ModuleHandler::Module* mod = module_handler.GetModule(2);
  } catch(const std::out_of_range& oor) {
    mod_exist = false;
  }

  TEST_ASSERT_TRUE_MESSAGE(mod_exist, "Module does not exist after register");
}

void test_Connect(void) {

}

void test_Post(void) {

}

void test_DeregisterModule(void) {
  module_handler.RegisterModule(Esp32Command_wifi_command_tag);

  bool mod_exist = true;

  // check that module doesn't exist anymore
  try {
    // get the module of that message type
    ModuleHandler::Module* mod = module_handler.GetModule(2);
  } catch (const std::out_of_range& oor) {
    mod_exist = false;
  }

  TEST_ASSERT_TRUE_MESSAGE(mod_exist, "Module exists after deregister");
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(test_true);

  UNITY_END();
}

void loop() {}