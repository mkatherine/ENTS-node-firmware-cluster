[![Build stm32](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/stm32.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/stm32.yaml) [![Build esp32](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/esp32.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/esp32.yaml) [![Doxygen](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/docs.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/docs.yaml)

# soil-power-sensor-firmware

Firmware for STM32 and ESP32 microcontrollers on the Soil Power Sensor board along with supporting libraries.

## Structure of project

| Path | Description | Link |
| --- | --- | --- |
| `stm32/` | Platformio project for the stm32-based Wio-E5 mini | @ref ./stm32/README.md |

### Protocol Buffers

[protbuf](@ref ./stm32/README.md)

## Generation documentation

This project use [Doxygen](https://www.doxygen.nl/) for its code documentation. HTML documentation is automatically generated through Github Actions and is updated whenever there is a change to the `main` branch. To generate documentation locally in the `docs/` folder, run the following from the root directory:

```bash
doxygen Doxyfile
```

Online version of the documentation for the `main` branch is available at https://jlab-sensing.github.io/soil-power-sensor-firmware/