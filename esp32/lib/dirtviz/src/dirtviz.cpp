/**
 * @see dirtviz.hpp
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-11-29
*/

#include "dirtviz.hpp"

Dirtviz::Dirtviz(void)
{

}

Dirtviz::Dirtviz(const std::string& url, const uint16_t& port)
{

}

void Dirtviz::SetUrl(const std::string& new_url)
{

}

std::string Dirtviz::GetUrl(void) const
{
  return std::string();
}

void Dirtviz::SetPort(const uint16_t &new_port)
{
}

uint16_t Dirtviz::GetPort(void) const
{
  return 0;
}

size_t Dirtviz::SendMeasurement(const uint8_t *meas, size_t meas_len,
                                uint8_t *resp)
{
  return size_t();
}
