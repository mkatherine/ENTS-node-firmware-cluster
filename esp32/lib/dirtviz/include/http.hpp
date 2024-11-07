#ifndef LIB_DIRTVIZ_INCLUDE_HTTP_H_
#define LIB_DIRTVIZ_INCLUDE_HTTP_H_

#include <string>
#include <map>

class HttpClient {
  public:
    /**
     * @brief Default constructor
     *
     * @param Http response string
     */
    HttpClient(const std::string &resp);

    /**
     * @brief Default destructor
     */
    ~HttpClient();

    /**
     * @brief Get the Http version
     *
     * Example return value "Http/1.1"
     *
     * @retrun Http version
     */
    std::string Version();

    /**
     * @brief Get the http response code
     *
     * @return Http response code
     */
    unsigned int ResponseCode();

    /**
     * @brief Get value of header
     *
     * @param header Requested header
     *
     * @return Value of header
     */
    std::string Header(std::string header);

    /**
     * @brief Get data from Http request
     *
     * @return String of data
     */
    std::string Data();

  private:
    /** Http version */
    std::string version;

    /** Http response code */
    unsigned int code;

    /** Map for requested headers */
    std::map<std::string, std::string> headers;

    /** Http data */
    std::string data;

    const std::string LINE_END = "\r\n";
    /**
     * @brief Decode status string
     *
     * Expected format:
     * Http/1.1 200
     *
     * @param status Status string
     */
    void DecodeStatus(std::string status_str);

    /**
     * @brief Decode header
     *
     * Expected format:
     * Content-Type: application/octet-stream
     * ...
     *
     * @param headers Headers string
     */
    void DecodeHeaders(std::string headers_str);

    /**
     * @brief Remove leading spaces from a string
     *
     * @param str Input string
     */
    void RemoveLeadingSpace(std::string &str);

    /**
     * @brief Remove trailing spaces from a string
     *
     * @param str Input string
     */
    void RemoveTrailingSpace(std::string &str);

    /**
     * @brief Remove leading and trailing spaces from the string
     *
     * @param str Input string
     */
    void ExtractText(std::string &str);

    /**
     * @brief Remove all spaces from string
     *
     * @param str Input string
     */
    void RemoveSpace(std::string &str);
};

#endif  // LIB_DIRTVIZ_INCLUDE_HTTP_H_
