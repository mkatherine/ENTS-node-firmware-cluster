/**
 * @file template_module.hpp
 * @author John Madden (jmadden173@pm.me)
 * @brief 
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ESP32_LIB_MODULE_HANDLER_INCLUDE_TEMPLATE_MODULE_H_
#define ESP32_LIB_MODULE_HANDLER_INCLUDE_TEMPLATE_MODULE_H_

#include <cstddef>
#include <cstddef>

namespace ModuleHandler {
  class Module {
    public:

    /**
     * @brief Construct a new Module object
     * 
     */
    Module(void);

    /**
     * @brief Destroy the Module object
     * 
     */
    ~Module(void);

    /**
     * @brief Arduino I2C onRecieve
     * 
     * @param num_bytes Number of bytes received from controller
     */
    virtual void OnReceive(size_t num_bytes);

    /**
     * @brief Arduino I2C onRequest
     * 
     */
    virtual void OnRequest(void);
    
    /**
     * @brief Get the current state of the module
     * 
     * @return Integer of state enum
     */
    virtual int State(void);

    /**
     * @brief Resets the module to default state
     * 
     */
    virtual void Reset();

    protected:
    /**
     * @brief Decodes a protobuf message
     * 
     */
    void DecodeMessage(void);
  };
}

#endif // ESP32_LIB_MODULE_HANDLER_INCLUDE_TEMPLATE_MODULE_H_