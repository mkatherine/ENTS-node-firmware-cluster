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
#include <cstdint>
#include <map>

#include "template_module.hpp"

#include "transcoder.h"

// buffers that are used in place of i2c communication
#ifdef UNIT_TEST
static uint8_t module_handler_rx_buffer[32] = {}
static uint8_t module_handler_tx_buffer[32] = {};
#endif

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
    void RegisterModule(int type, Module& module);

    /**
     * @brief Deregister a module
     * 
     * @param type Message type of module
     */
    void DeregisterModule(int type);

    /**
     * @brief Get the module associated with a specific message type
     * 
     * @param type Message type
     * 
     * @returns Reference to module associated with the type
     */
    Module& GetModule(int type);

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
     * Due to the I2C buffer size on Arduino, the buffer passed to the module is
     * limited to 32 bytes.
     */
    void OnRequest(void);

    private:

    /** Map of request types to modules */
    std::map<int, Module&> req_map;

    /** Store reference to last module */
    Module& last_module;
  };
}

#endif // ESP32_LIB_MODULE_HANDLER_INCLUDE_MODULE_HANDLER_H_