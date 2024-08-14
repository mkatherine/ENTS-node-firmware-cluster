/**
 * @file test_main.cpp
 * @author John Madden (jmadden173@pm.me)
 * @brief 
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <unity.h>

#include "module_handler.hpp"
#include "module_test.hpp"
  
// create handler
ModuleHandler module_handler;

// test module
ModuleTest module_test;

void setUp(void) {}

void tearDown(void) {}

void TestRegister(void) {
  module_handler.RegisterModule(module_test, 0);
}

void TestReset(void) {
  module_handler.ResetModule(module_test);
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);


  UNITY_BEGIN();

  RUN_TEST(TestModule)

  UNITY_END();
}

void loop() {}