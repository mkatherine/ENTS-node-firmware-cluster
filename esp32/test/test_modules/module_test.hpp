#ifndef ESP32_TEST_TEST_MODULE_MODULE_TEST_HPP_
#define ESP32_TEST_TEST_MODULE_MODULE_TEST_HPP_

#include "template_module.hpp"

class ModuleTest: public ModuleHandler::Module {
  public:
  ModuleTest(void);
  ~ModuleTest(void);

  unsigned int CheckState(void);

  private:

  typedef enum {
    INIT = 0,
    RECEIVE = 1,
    REQUEST = 2
  } StateEnum;

  StateEnum state = INIT;
};

#endif // ESP32_TEST_TEST_MODULE_MODULE_TEST_HPP_