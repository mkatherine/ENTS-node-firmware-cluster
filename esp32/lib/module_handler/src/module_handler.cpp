#include "module_handler.hpp"

#include <cstring>

#include <Arduino.h>
#include <Wire.h>

#include "transcoder.h"

ModuleHandler::ModuleHandler::ModuleHandler() {}

ModuleHandler::ModuleHandler::~ModuleHandler() {}

void ModuleHandler::ModuleHandler::RegisterModule(int type, Module *module)
{
  // add module to map
  this->req_map[type] = module;
}

void ModuleHandler::ModuleHandler::DeregisterModule(int type) {
  // remove module from map
  this->req_map.erase(type);
}

void ModuleHandler::ModuleHandler::OnReceive(size_t num_bytes) {
  uint8_t buffer[num_bytes] = {};

  #ifdef UNIT_TEST
  std::memcpy(buffer, module_handler_rx_buffer, num_bytes);
  #else
  for (int i = 0; i < num_bytes; i++) {
    // check if available
    if (!Wire.available()) {
      Serial.printf("Specified number of bytes is not available");
    }

    // read bytes
    buffer[i] = Wire.read();
  }
  #endif

  // decode measurement
  Esp32Command cmd = DecodeEsp32Command(buffer, num_bytes);

  // store reference to module for future OnRequest calls
  this->last_module = this->req_map.at(cmd.which_command);
  // forward command to module
  this->last_module->OnReceive(cmd);
}

void ModuleHandler::ModuleHandler::OnRequest(void) {
  // create buffer with same size of arduino
  size_t len = 0;
  uint8_t buffer[32];

  // forward request to last module received
  len = this->last_module->OnRequest(buffer);

  #ifdef UNIT_TEST
  std::memcpy(module_handler_tx_buffer, buffer, len);
  #else
  // wire to i2c
  Wire.write(buffer, len);
  #endif
}

ModuleHandler::Module* ModuleHandler::ModuleHandler::GetModule(int type) {
  return this->req_map.at(type);
}

void ModuleHandler::ModuleHandler::ResetModules(void) {
  // call Reset() for all modules in req_map
  for (auto& entry : this->req_map) {
    entry.second->Reset();
  }
}