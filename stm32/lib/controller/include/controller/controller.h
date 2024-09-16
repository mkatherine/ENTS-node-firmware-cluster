#ifndef STM32_LIB_CONTROLLER_INC_CONTROLLER_H_
#define STM32_LIB_CONTROLLER_INC_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the esp32 controller
 * 
 * Currently nothing is done as the i2c peripherial is expected to already be
 * initialized.In the future a quick communication check will happen.
 */
void ControllerInit(void);

/**
 * @brief Deinitialize the esp32 controller
 * 
 */
void ControllerDeinit(void);

#ifdef __cplusplus
}
#endif

#endif // STM32_LIB_CONTROLLER_INC_CONTROLLER_H_