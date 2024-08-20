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
static ModuleHandler::ModuleHandler module_handler;
// create test module
static ModuleTest module_test;

void setUp(void) {}

void tearDown(void) {}

void TestRegister(void) {
  // register module with message
  module_handler.RegisterModule(0, module_test);

  bool mod_exist = true;

  // check if module exists in map
  try {
    ModuleHandler::Module& mod = module_handler.GetModule(0);
  } catch(const std::out_of_range& oor) {
    mod_exist = false;
  }

  TEST_ASSERT_TRUE_MESSAGE(mod_exist, "Module does not exist after register");
}

void TestDeregister(void) {
  module_handler.DeregisterModule(0);

  bool mod_exist = true;

  // check that module doesn't exist anymore
  try {
    // get the module of that message type
    ModuleHandler::Module& mod = module_handler.GetModule(0);
  } catch (const std::out_of_range& oor) {
    mod_exist = false;
  }

  TEST_ASSERT_TRUE_MESSAGE(mod_exist, "Module exists after deregister");
}

void TestReset(void) {
  // reset modules
  module_handler.ResetModules();

  // get reference to module
  ModuleHandler::Module& mod = module_handler.GetModule(0);

  // check for the 0th state
  TEST_ASSERT_EQUAL(0, mod.State());
}

void TestOnReceive(void) {

}

void TestOnRequest(void) {

}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);


  UNITY_BEGIN();

  RUN_TEST(TestRegister);
  RUN_TEST(TestDeregister);
  RUN_TEST(TestReset);

  UNITY_END();
}

void loop() {}