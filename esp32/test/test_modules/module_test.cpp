#include "module_test.hpp"

ModuleTest::ModuleTest(void) {}

ModuleTest::~ModuleTest(void) {}

void ModuleTest::OnReceive(const Esp32Command& cmd) {
  if (cmd.command.test_command.state == TestCommand_ChangeState_RECEIVE) {
    // set state to receive
    this->state = RECEIVE;
  } else if (cmd.command.test_command.state == TestCommand_ChangeState_REQUEST) {
    // set state to request
    this->state = REQUEST;
    // store data in buffer
    this->Int32ToByteArray(cmd.command.test_command.data, this->buffer);
  }
  
}

size_t ModuleTest::OnRequest(uint8_t* buffer) {
  this->state = REQUEST;
}

int ModuleTest::State(void) {
  return this->state;
}

void ModuleTest::Int32ToByteArray(int32_t value, uint8_t* buffer) {
  // Extract the highest byte
  buffer[0] = (value >> 24) & 0xFF;
  // Extract the second highest byte
  buffer[1] = (value >> 16) & 0xFF;
  // Extract the second lowest byte
  buffer[2] = (value >> 8) & 0xFF;
  // Extract the lowest byte
  buffer[3] = value & 0xFF;
}