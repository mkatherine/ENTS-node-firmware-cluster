#include "wifi.h"

#include <stm32_systime.h>

#include "sys_app.h"
#include "sensors.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "fifo.h"
#include "controller/wifi.h"
#include "userConfig.h"

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
 * @param context Timer context.
 */
void UploadEvent(void *context);

/**
 * @brief Upload data to hub
 * 
 * Posts data to configured url.
 */
void Upload(void);

/**
 * @brief Connect to WiFi
 *
 * Connects to WiFi network and sets the system time. Code will block and
 * reattempt connection until successful.
 */
void Connect(void);

/**
 * @brief Timesync with NTP server
 */
void TimeSync(void);

/**
 * @brief Check API health
 */
void Check(void);

/**
 * @brief Setup upload timer
 * 
 * Start sensor measurements and setup upload task/timer.
 */
void StartUploads(void);

void WiFiInit(void) {
  APP_LOG(TS_OFF, VLEVEL_M, "WiFi app starting\r\n");

  // connect to WiFi
  Connect();
  // sync with NTP server
  TimeSync();
  // API health check
  Check();
  // start timers for uploading
  StartUploads();
}

void UploadEvent(void *context) {
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_WiFiUpload), CFG_SEQ_Prio_0);
}

void Upload(void) {
  const size_t buffer_size = 256;
  size_t buffer_len = 0;
  uint8_t buffer[buffer_size];

  // get buffer data
  FramStatus status = FramGet(buffer, &buffer_len);
  if (status != FRAM_OK) {
    if (status == FRAM_BUFFER_EMPTY) {
      APP_LOG(TS_OFF, VLEVEL_M, "Buffer empty!\r\n")
    } else {
      APP_LOG(TS_OFF, VLEVEL_M,
          "Error getting data from fram buffer. FramStatus = %d\r\n", status);
    }
    return;
  }

  // print buffer
  APP_LOG(TS_ON, VLEVEL_M, "Payload[%d]: ", buffer_len);
  for (int i = 0; i < buffer_len; i++)
  {
    APP_LOG(TS_OFF, VLEVEL_M, "%x ", buffer[i]);
  }
  APP_LOG(TS_OFF, VLEVEL_M, "\r\n");

  // posts data to website
  int http_code = 0;
  do {
    APP_LOG(TS_ON, VLEVEL_M, "Uploading data...\t");
    uint8_t resp[256];
    size_t resp_len = 0;
    http_code = ControllerWiFiPost(buffer, buffer_len, resp, &resp_len);
    APP_LOG(TS_OFF, VLEVEL_M, "%d\r\n", http_code);
    
    // reconnect and retry if general error
    if (http_code != 200) {
      APP_LOG(TS_ON, VLEVEL_M, "Error with WiFi connection!\r\n");
      APP_LOG(TS_ON, VLEVEL_M, "Attempting Reconnect\t");
      
      Connect();
    }
  } while (http_code == 0);
}

void Connect(void) {
  //  load configurations
  const UserConfiguration* cfg = UserConfigGet();

  //const char ssid[] = "UCSC-Devices";
  //const char* passwd = "hqWeRfvsn7eLd7MPrW";
  
  const char ssid[] = "HARE_Lab";
  const char* passwd = "";


  // initialize
  unsigned int num_tries = 0;

  uint8_t wifi_status = 0;

  while (wifi_status != 3) {
    //APP_LOG(TS_OFF, VLEVEL_M, "Connecting to %s...", cfg->WiFi_SSID);
    //wifi_status = ControllerWiFiInit(cfg->WiFi_SSID, cfg->WiFi_Password);
    APP_LOG(TS_OFF, VLEVEL_M, "Connecting to %s...", ssid);
    wifi_status = ControllerWiFiInit(ssid, passwd);

    if (wifi_status == 3) {
      APP_LOG(TS_OFF, VLEVEL_M, "Connected!\r\n");
    } else {
      APP_LOG(TS_OFF, VLEVEL_M, "Attempt %d failed!\r\n", num_tries);
      APP_LOG(TS_OFF, VLEVEL_M, "Retrying in 10 seconds...\r\n");
      HAL_Delay(10000);
    }

    ++num_tries;
  }
}

void StartUploads(void) {
  // start sensor measurements
  APP_LOG(TS_ON, VLEVEL_M, "Starting sensor measurements...\t");
  SensorsStart();
  APP_LOG(TS_OFF, VLEVEL_M, "Started!\r\n");

  // setup upload task
  APP_LOG(TS_ON, VLEVEL_M, "Starting upload task...\t")
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_WiFiUpload), UTIL_SEQ_RFU, Upload);
  UTIL_TIMER_Create(&UploadTimer, UploadPeriod, UTIL_TIMER_PERIODIC, UploadEvent, NULL);
  UTIL_TIMER_Start(&UploadTimer);
  APP_LOG(TS_OFF, VLEVEL_M, "Started!\r\n");
}

void TimeSync(void) {
  SysTime_t ts = {.Seconds = -1, .SubSeconds = 0};

  APP_LOG(TS_OFF, VLEVEL_M, "Syncing time...\t");
  ts.Seconds = ControllerWiFiTime();
  SysTimeSet(ts);
  APP_LOG(TS_OFF, VLEVEL_M, "Done!\r\n");
  APP_LOG(TS_OFF, VLEVEL_M, "Current timestamp is %d\r\n", ts.Seconds);
}

void Check(void) {
  const UserConfiguration* cfg = UserConfigGet();
  
  const char url[] = "dirtviz.jlab.ucsc.edu";
  const uint32_t port = 80;

  APP_LOG(TS_ON, VLEVEL_M, "Checking API health...\t");
  unsigned int http_code = ControllerWiFiCheck(url, port);
  APP_LOG(TS_OFF, VLEVEL_M, "%d\r\n", http_code);
}
