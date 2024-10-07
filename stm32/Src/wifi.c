#include "wifi.h"

#include <stm32_systime.h>

#include "sys_app.h"
#include "sensors.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "fifo.h"
#include "controller/wifi.h"

/**
 * @brief Timer for uploads
 * 
 */
static UTIL_TIMER_Object_t UploadTimer = {};

/**
 * @brief Period between uploads
 * 
 */
static UTIL_TIMER_Time_t UploadPeriod = 10000;

/**
 * @brief Function call for upload event
 * 
 * @param context Timer context
 */
void UploadEvent(void *context);

/**
 * @brief Upload data to hub
 * 
 * Posts data to configured url 
 */
void Upload(void);

void WiFiInit(void) {
  // initialize the app
  SystemApp_Init();

  APP_LOG(TS_OFF, VLEVEL_M, "WiFi app starting\r\n");


  // TODO load configuration
  const char ssid[] = "HARE_Lab";
  const char* passwd = NULL;
  const char url[] = "https://httpbin.org/get";
  const uint32_t port = 443;

  APP_LOG(TS_OFF, VLEVEL_M, "Connecting to %s...\t");

  // initialize 
  SysTime_t ts = {.Seconds = 0, .SubSeconds = 0};
  ts.Seconds = ControllerWiFiInit(ssid, passwd, url, port);

  APP_LOG(TS_OFF, VLEVEL_M, "Connected!\r\n");
  APP_LOG(TS_OFF, VLEVEL_M, "Current timestamp is %d", ts.Seconds);

  // update clock
  SysTimeSet(ts);

  // start sensor measurements
  APP_LOG(TS_ON, VLEVEL_M, "Starting sensor measurements...\t");
  SensorsStart();
  APP_LOG(TS_ON, VLEVEL_M, "Started!\r\n");

  // setup upload task
  APP_LOG(TS_ON, VLEVEL_M, "Starting upload task...\t")
  UTIL_SEQ_RegTask((1 >> CFG_SEQ_Task_WiFiUpload), UTIL_SEQ_RFU, Upload);
  UTIL_TIMER_Create(&UploadTimer, UploadPeriod, UTIL_TIMER_PERIODIC, UploadEvent, NULL);
  UTIL_TIMER_Start(&UploadTimer);
  APP_LOG(TS_ON, VLEVEL_M, "Started!\r\n");
}

void UploadEvent(void *context) {
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_WiFiUpload), CFG_SEQ_Prio_0);
}

void Upload(void) {
  const size_t buffer_size = 256;
  size_t buffer_len = 0;
  uint8_t buffer[buffer_size];

  FramStatus status = FramGet(buffer, &buffer_len);
  if (status != FRAM_OK) {
    APP_LOG(TS_OFF, VLEVEL_M,
            "Error getting data from fram buffer. FramStatus = %d", status);
  }

  // print buffer
  APP_LOG(TS_ON, VLEVEL_M, "Payload[%d]: ", buffer_len);
  for (int i = 0; i < buffer_len; i++)
  {
    APP_LOG(TS_OFF, VLEVEL_M, "%x ", buffer[i]);
  }
  APP_LOG(TS_OFF, VLEVEL_M, "\r\n");

  // posts data to website
  APP_LOG(TS_ON, VLEVEL_M, "Uploading data...\t");
  uint8_t resp[256];
  size_t resp_len = 0;
  int http_code = 0;
  http_code = ControllerWiFiPost(buffer, buffer_len, resp, &resp_len);
  APP_LOG(TS_OFF, VLEVEL_M, "%d", http_code);
}