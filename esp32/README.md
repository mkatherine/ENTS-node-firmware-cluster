# ESP32 Firmware

The ESP32 firmware acts as a WiFi interface the STM32 module. It gets binary data through the I2C connection with the ESP32 and uploads it to a configured web address. The binary response is then passed back to the STM32 for decoding. The ESP32 handles *only* the WiFi interface. It is up to the STM32 to check all errors relating to decoding binary data.

## Flashing

TODO

## Testing

We have included the python script `tools/http_server.py` to enable testing the WiFi interface of the ESP32. By default the server simulates a success POST request, returning a `200` response. The flag `--error` allows for a `418` to test error handling. See `http_server.py --help` for more usage information. The steps for testing are as follows:

1. Start HTTP server with
```bash
python http_server.py
```

2. **Update connection parameters for the WiFi network and server address.** The WiFi network should be the same network that your device connected to. The server address should be the IP address of your device hosting the server. By default the port is `8080`.

```c++
const char ssid[] = "my-ssid";
const char pass[] = "pass";

Dirtviz api("dirtviz.jlab.ucsc.edu", 443);
```

3. **Flash the ESP32.** Use the following command to select the correct environment for `example_dirtviz.cpp`. See [Flashing](#flashing) for more information.

```bash
pio run -e example_dirtviz -t upload
```

4. Monitor the server and serial output to ensure data is being properly sent.