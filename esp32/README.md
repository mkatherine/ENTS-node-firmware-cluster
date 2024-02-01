# ESP32 Firmware

The firmware for the ESP32 provides a way to upload data through the simpler WiFi interface. The ESP32 receives serialized data through its I2C interface with the STM32 and forwards the data to an endpoint. Therefore the ESP32 is not required to have knowledge of the protocol buffers.