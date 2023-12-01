/**
 * @brief Library used to interface with the Dirtviz API
 * 
 * Assumes that WiFi interface is connected to the network.
 * 
 * @author John Madden <jmadden173@pm.me>
 * @date 2023-11-29
*/

#include <string>

#include <WiFi.h>

/**
 * @brief HTTP interface for Dirtviz API
*/
class Dirtviz
{
private:
  /** URL of API */
  std::string url;

  /** Port of API*/
  uint16_t port;

  /** WiFi client for connection with API*/
  WiFiClient client;

public:
  /**
   * @brief Default constructor
  */
  Dirtviz(void);

  /**
   * @brief Default constructor
   * 
   * Allows for setting of URL on initialization.
   * 
   * @param url API URL
   */ 
  Dirtviz(const std::string& url);

  /**
   * @brief Setter for @p url
   * 
   * @param new_url New API URL
  */
  void SetPowerUrl(const std::string& new_url);

  /**
   * @brief Getter for @p url
   * 
   * @returns Value of @p url
  */
  std::string GetPowerUrl(void) const;

  /**
   * @brief Send serialized measurement to the API
   * 
   * @param[in] meas Pointer to serialized measurement data
   * @param[in] meas_len Number of bytes in @p meas
   * @param[out] resp Serialized response from server
   * 
   * @return Length of @p resp
  */
  size_t SendMeasurement(const uint8_t* meas, size_t meas_len,
                         uint8_t* resp);
};