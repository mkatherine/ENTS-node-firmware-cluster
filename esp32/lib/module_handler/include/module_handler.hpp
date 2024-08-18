/**
 * @file module_handler.hpp
 * @author John Madden (jmadden173@pm.me)
 * @brief 
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULE_HANDLER_H_
#define ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULE_HANDLER_H_

#include <cstddef>

#include "template_module.hpp"

namespace ModuleHandler {
  class ModuleHandler {
    public:
    /**
     * @brief Construct a new Module Handler object
     * 
     */
    ModuleHandler();

    /**
     * @brief Destroy the Module Handler object
     * 
     */
    ~ModuleHandler();
    
    /**
     * @brief Register a module with a message type
     * 
     * @param module Reference to module object
     * @param type Message type
     */
    void RegisterModule(Module& module, int type);

    /**
     * @brief Deregister a module
     * 
     * @param module Reference to module object
     */
    void DeregisterModule(Module& module);

    /**
     * @brief Get the module associated with a specific message type
     * 
     * @param type Message type
     * 
     * @returns Reference to module associated with the type
     */
    const Module& GetModule(int type);

    /**
     * @brief Resets all modules
     * 
     */
    void ResetModules(void);

    /**
     * @brief Arduino I2C onRecieve
     * 
     * @param num_bytes Number of bytes received from controller
     */
    void OnReceive(size_t num_bytes);

    /**
     * @brief Arduino I2C onRequest
     * 
     */
    void OnRequest(void);

    /**
     * @brief Run the module handler
     * 
     */
    void Run(void);

    private:

  };
}

#endif // ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULE_HANDLER_H_