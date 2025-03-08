#include "teros12.h"

#include "stm32_systime.h"
#include "userConfig.h"

SDI12Status Teros12ParseMeasurement(const char *buffer, Teros12Data *data) {
  // parse string and check number of characters parsed
  int rc = sscanf(buffer, "%1c+%f%f+%d", &data->addr, &data->vwc, &data->temp,
                  &data->ec);
  if (rc < 4) {
    return SDI12_PARSING_ERROR;
  }

  return SDI12_OK;
}

SDI12Status Teros12GetMeasurement(char addr, Teros12Data *data) {
  // buffer to store measurement
  // based on the measurement range of the device the max string length is
  // 0+1846.16+22.3+20000 = 20
  char buffer[20];

  // status messages
  SDI12Status status = SDI12_OK;

  // get measurement string
  // Measured 130ms experimentally, set to 200 ms to be safe
  SDI12_Measure_TypeDef measurment_info;
  status = SDI12GetMeasurment((uint8_t)addr, &measurment_info, buffer, 1000);
  if (status != SDI12_OK) {
    return status;
  }

  // parse measurement into data structure
  size_t buffer_len = 5;
  status = Teros12ParseMeasurement(buffer, data);
  if (status != SDI12_OK) {
    return status;
  }

  return status;
}

size_t Teros12Measure(uint8_t *data) {
  // get timestamp
  SysTime_t ts = SysTimeGet();

  Teros12Data sens_data = {};
  SDI12Status status = Teros12GetMeasurement('0', &sens_data);
  if (status != SDI12_OK) {
    return -1;
  }

  const UserConfiguration *cfg = UserConfigGet();

  // calibration equation for mineral soils from Teros12 user manual
  // https://publications.metergroup.com/Manuals/20587_TEROS11-12_Manual_Web.pdf?_gl=1*174xdyp*_gcl_au*MTIxODkwMzcuMTc0MTIwMjU3Nw..
  float vwc_adj = (3.879e-4 * sens_data.vwc) - 0.6956;

  size_t data_len = EncodeTeros12Measurement(
      ts.Seconds, cfg->logger_id, cfg->cell_id, sens_data.vwc, vwc_adj,
      sens_data.temp, sens_data.ec, data);

  return data_len;
}
