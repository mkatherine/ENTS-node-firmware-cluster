/**
 * @see dirtviz.hpp
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-11-29
*/

#include "dirtviz.hpp"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <sstream>
#include <iomanip>

Dirtviz::Dirtviz(void) : url(nullptr) {}

Dirtviz::Dirtviz(const char *url, const uint16_t &port)
{
  // set parameters
  this->SetUrl(url);
  this->SetPort(port);
}

Dirtviz::~Dirtviz()
{
  // free memory
  free(this->url);
}

void Dirtviz::SetUrl(const char *new_url)
{
  // get length of new url string, add 1 for null char
  size_t url_len = strlen(new_url);
  ++url_len;

  // allocate memory
  char * temp_url = (char *) realloc(this->url, url_len);

  if (temp_url != nullptr) {
    this->url = temp_url;
    strcpy(this->url, new_url); // strcpy is safe here because we just allocated enough space
  } else {
    // Handle allocation failure (e.g., set an error flag, use a default URL, etc.)
    
  }
}

const char *Dirtviz::GetUrl(void) const
{
  return this->url; 
}

void Dirtviz::SetPort(const uint16_t &new_port)
{
  this->port = new_port;
}

uint16_t Dirtviz::GetPort(void) const
{
  return this->port;
}

uint32_t Dirtviz::Check() const {
  Log.traceln("Dirtviz::Check");
  WiFiClient client;

  if (!client.connect(url, port)) {
    return 0;
  }

  Log.traceln("Sending GET request");

  // format request
  std::ostringstream req;
  req << "GET /api/ HTTP/1.1" << "\r\n";
  req << "Host: " << url << "\r\n";
  req << "User-Agent: curl/8.10.1" << "\r\n";
  req << "\r\n";

  // send full request to server
  client.println(req.str().c_str());

  //client.println("GET /api/ HTTP/1.1");
  //client.println("Host: dirtviz.jlab.ucsc.edu");
  //client.println("User-Agent: curl/8.10.1");
  //client.println();

  Log.traceln("Done!");

  // wait until there's bytes available
  while (!client.available()) {
    Log.traceln("Delaying until available");
    delay(10);
  }

  // read response
  std::string resp;
  while (client.available()) {
    char c = client.read();
    resp += c;
  }
  
  // close connection
  client.flush();
  client.stop();  
 
  // read string into an object
  HttpClient http_client(resp);

  unsigned int http_code = http_client.ResponseCode();
  if (http_code != 200) {
    Log.warningln("Api health check failed! Reponse code: %d", http_code);
  }
 
  std::string date_str = http_client.Header("Date");
  std::stringstream date_stream(date_str);
  std::tm date = {0};
  date_stream >> std::get_time(&date, "%a, %d %b %Y %H:%M:%S GMT");

  std::time_t unix_epochs = std::mktime(&date);

  return (uint32_t) unix_epochs;
}

HttpClient Dirtviz::SendMeasurement(const uint8_t *meas, size_t meas_len) {
  WiFiClient client;

  char buffer[100];

  // connect to server
  if (!client.connect(url, port)) {
    Log.errorln("Connection to %s:%d failed!", url, port);
    HttpClient empty("");
    return empty;
  }

  // send data

  // HTTP command, path, and version
  client.println("POST /api/ HTTP/1.1");
  // who we are posting to
  client.print("Host: ");
  client.print(this->url);
  client.print(":");
  client.println(this->port);
  // type of data
  client.println("Content-Type: application/octet-stream");
  // length of data (specific to application/octet-stream)
  sprintf(buffer, "Content-Length: %d", meas_len);
  client.println(buffer);
  // close connection after data is sent
  client.println("Connection: close");
  // newline indicating end of headers
  client.println();
  // send data
  for (int idx = 0; idx < meas_len; ++idx)
  {
    client.write(meas[idx]);
  }

  // read response

  // wait until there's bytes available
  while (!client.available()) {
    Log.traceln("Delaying until available");
    delay(10);
  }

  // read response
  std::string resp;
  while (client.available()) {
    char c = client.read();
    resp += c;
  }
  
  // close connection
  client.flush();
  client.stop();  
 
  // read string into an object
  HttpClient http_client(resp);
  
  return http_client;
}

