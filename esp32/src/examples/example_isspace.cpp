/**
 * @file example_http.cpp
 * @author John Madden <jmadden173@pm.me>
 * @brief Testing file for HttpClient
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 * 
 */

#include <clocale>
#include <ctype.h>
#include <Arduino.h>
#include <ArduinoLog.h>

void setup() {
  Serial.begin(115200);

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  std::setlocale(LC_ALL, "en_US.UTF-8");
  Log.traceln("Current locale: %s", std::setlocale(LC_ALL, NULL));

  for (unsigned char i=0; i < 127; i++) {
    Log.traceln("isspace(%C) = %T", i, isspace(i));
  }
}

void loop() {}
