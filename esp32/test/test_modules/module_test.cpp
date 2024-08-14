#include "module_test.hpp"

ModuleTest::ModuleTest(void) {}

ModuleTest::~ModuleTest(void) {}

ModuleTest::OnReceive(size_t num_bytes) {
  this->state = RECEIVE;
}

ModuleTest::OnRequest(void) {
  this->state = REQUEST;
}

unsigned int ModuleTest::CheckState(void) {
  return this->state;
}