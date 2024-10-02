#include "fram.h"

#include "fm24cl16b.h"

#define FRAM_FM24CL16B

FramStatus FramWrite(uint16_t addr, const uint8_t *data, uint8_t len) {
#ifdef FRAM_FM24CL16B
  return Fm24cl16bWrite(addr, data, len);
#else
  return FRAM_OK;
#endif
}

FramStatus FramRead(uint16_t addr, uint8_t len, uint8_t *data) {
#ifdef FRAM_FM24CL16B
  return Fm24cl16bRead(addr, len, data);
#else
  return FRAM_OK;
#endif
}

HAL_StatusTypeDef ConfigureSettings(configuration c) {
  HAL_StatusTypeDef status = HAL_OK;

  // TODO(GSOC student) implement user config write

  return status;
}

configuration ReadSettings(void) {
  configuration c;

  // TODO(GSOC student) implement user config read

  return c;
}
