/**
 * @brief Main file for the Soil Power Sensor firmware
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-11-28
 */

#include <Arduino.h>

#include <WiFi.h>

/** Baud rate for serial interface */
#define SERIAL_BAUD 115200


const char ssid[] = "my-ssid";
const char pass[] = "pass";

wl_status_t rc;

/**
 * @brief Initialization code run on startup
 * 
 * 
*/
void setup()
{
  #if DEBUG 
  // Start serial interface
  Serial.begin(115200);
  // Wait for serial connection
  while (!Serial) { delay(100); }

  Serial.println("Connect to SSID: " + ssid + ", PASS: " + pass);
  #endif

  // Connect to WiFi network
  WiFi.begin(ssid, pass);

  // Wait for WiFi to connect
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

void loop()
{
  Serial.println(WiFi.status());
}
