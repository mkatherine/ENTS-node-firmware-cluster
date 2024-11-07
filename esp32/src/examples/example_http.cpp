/**
 * @file example_http.cpp
 * @author John Madden <jmadden173@pm.me>
 * @brief Testing file for HttpClient
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <locale.h>

#include "dirtviz.hpp"

void setup() {
  Serial.begin(115200);

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  setlocale(LC_ALL, "en_US.UTF-8");
  Log.traceln("Current locale: %s", setlocale(LC_ALL, NULL));

  for (unsigned char i=0; i < 127; i++) {
    Log.traceln("std::isprint(%C) = %T", i, std::isprint(i));
    Log.traceln("isprint(%C) = %T", i, isprint(i));
  }

  Serial.println("Input message");

//  std::string resp =
//R""(
//HTTP/2 200\r\n
//content-type: application/json
//content-length: 59
//date: Fri, 01 Nov 2024 23:53:02 GMT
//access-control-allow-origin: *
//server: gunicorn
//x-cache: Miss from cloudfront
//via: 1.1 4a628119373a320de50f1e6bc0e5f87a.cloudfront.net (CloudFront)
//x-amz-cf-pop: SFO5-P2
//x-amz-cf-id: t9EARJ8Hsu4FipZqiiVhsjwEU29sICEwjA7XMJNIOEXgOnJA-TlhdA==
//
//{
//    "hello": "I'm alive and healthy! Super healthy :D"
//}
//)"";

  std::string resp = "HTTP/2 200\r\n"
  "content-type: application/json\r\n"
  "content-length: 59\r\n"
  "date: Fri, 01 Nov 2024 23:53:02 GMT\r\n"
  "access-control-allow-origin: *\r\n"
  "server: gunicorn\r\n"
  "x-cache: Miss from cloudfront\r\n"
  "via: 1.1 4a628119373a320de50f1e6bc0e5f87a.cloudfront.net (CloudFront)\r\n"
  "x-amz-cf-pop: SFO5-P2\r\n"
  "x-amz-cf-id: t9EARJ8Hsu4FipZqiiVhsjwEU29sICEwjA7XMJNIOEXgOnJA-TlhdA==\r\n"
  "\r\n"
  "{\r\n"
  "    \"hello\": \"I'm alive and healthy! Super healthy :D\"\r\n"
  "}\r\n";

  Serial.println(resp.c_str());

  Serial.println("");
  Serial.println("Creating client");


  HttpClient client(resp);

  Serial.println("Success");
}

void loop() {}
