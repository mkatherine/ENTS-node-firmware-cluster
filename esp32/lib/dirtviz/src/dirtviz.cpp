/**
 * @see dirtviz.hpp
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-11-29
*/

#include "dirtviz.hpp"

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
  free(this->response);
}

void Dirtviz::SetUrl(const char *new_url)
{
  // get length of new url string, add 1 for null char
  size_t url_len = strlen(new_url);
  ++url_len;

  // allocate memory
  this->url = (char *) realloc(this->url, url_len);

  // copy string
  strcpy(this->url, new_url);
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

int Dirtviz::SendMeasurement(const uint8_t *meas, size_t meas_len)
{
  // WiFi client for connection with API
  WiFiClient client;

  // try connection return negative length if error
  if (!client.connect(this->url, this->port))
  {
    return -1;
  }

  // send data

  // HTTP command, path, and version
  client.println("POST [PATH] HTTP/1.1");
  // who we are posting to
  client.println("Host: [HOST]");
  // type of data
  client.println("Content-Type: application/octet-stream");
  // length of data (specific to application/octet-stream)
  client.println("Content-Length: [LENGTH]");
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
  
  // get length of response
  int resp_len = client.available();
  // allocate memory
  this->response = (char *) realloc(this->response, resp_len);

  // copy into buffer
  for (int idx = 0; idx < resp_len; ++idx)
  {
    this->response[idx] = client.read();
  }

  // disconnect after message is sent
  client.stop();


  // find status code
  int status_code;
  if (sscanf(this->response, "%*s %d", &status_code) != 1)
  {
    return -1;
  }

  return status_code;  
}

size_t Dirtviz::GetResponse(const uint8_t *data) const
{
  // find response length from header

  // get pointer to start of line
  const char *length_start = strstr(this->response, "Content-Length:");
  if (length_start == nullptr)
  {
    return 0;
  }

  // parse the length
  size_t data_len;
  if (sscanf(length_start, "%*s %u", &data_len))
  {
    return 0;
  }

  // read binary data, look for double CRLF
  data = (const uint8_t *) strstr(this->response, "\r\n\r\n");
  data += 4;

  // return the length of data
  return data_len;
}
