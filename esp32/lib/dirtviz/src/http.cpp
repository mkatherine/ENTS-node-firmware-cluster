#include "http.hpp"

#include <algorithm>
#include <sstream>

HttpClient::HttpClient(const std::string &resp) {
  // split up blocks of data
  size_t header_start = resp.find(LINE_END);
  size_t data_start = resp.find(LINE_END + LINE_END);

  std::string status_substr = resp.substr(0, header_start);
  DecodeStatus(status_substr);

  std::string headers_substr = resp.substr(header_start, data_start);
  DecodeHeaders(headers_substr);

  data = resp.substr(data_start);
}

HttpClient::~HttpClient() {}

std::string HttpClient::Version() { return version; }

unsigned int HttpClient::ResponseCode() { return code; }

std::string HttpClient::Header(std::string header) { return headers[header]; }

std::string HttpClient::Data() { return data; }

void HttpClient::DecodeStatus(std::string status_str) {
  std::stringstream status_stream(status_str);
  status_stream >> version >> code;
}

void HttpClient::DecodeHeaders(std::string headers_str) {
  std::stringstream header_stream(headers_str);
  std::string line;
  while (std::getline(header_stream, line)) {
    std::size_t pos = line.find(":");
    std::string key = line.substr(0, pos-1);
    std::string val = line.substr(pos);
    val.erase(std::remove_if(val.begin(), val.end(), [](char c) { return std::isspace(c); }));
    headers[key] = val; 
  }
}
