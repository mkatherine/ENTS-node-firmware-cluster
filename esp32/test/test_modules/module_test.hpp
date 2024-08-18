#ifndef ESP32_TEST_TEST_MODULE_MODULE_TEST_HPP_
#define ESP32_TEST_TEST_MODULE_MODULE_TEST_HPP_

#include "template_module.hpp"


class ModuleTest: public ModuleHandler::Module {
  public:
  ModuleTest(void);
  ~ModuleTest(void);

  uint8_t* input_buffer;
  uint8_t buffer[32];
  uint8_t output_buffer[32];

  private:

  typedef enum {
    INIT = 0,
    RECEIVE = 1,
    REQUEST = 2
  } StateEnum;

  StateEnum state = INIT;
};

#endif // ESP32_TEST_TEST_MODULE_MODULE_TEST_HPP_