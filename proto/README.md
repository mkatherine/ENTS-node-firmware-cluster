# Soil Power Sensor Protocol Buffers

[Protobuf](https://protobuf.dev/) is used to serialize measurement data when uploading data. Over LoRa this is strictly necessary due to payload size restrictions of the LoRaWAN protocol.

## C Implementation

On the *Wio-E5 mini* module, protocol buffers are implemented using [Nanopb](https://jpa.kapsi.fi/nanopb/) allowing for a plain-C implementation with minimal code size. The source files are structured into a PlatformIO library so the *stm32* project can build the source files using `lib_deps = soil_power_sensor_protobuf=symlink://../proto/c`. The source files for **Nanopb** were copied directly due to mismatching file structure. Additionally `timestamp.proto` was included from the **protobuf** to get compatable sources file for the well known `Timestamp` message type. A unit test is proved in `stm32/tests/test_proto`.

## Python module

TODO