[![Build stm32](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/stm32.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/stm32.yaml) [![Build esp32](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/esp32.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/esp32.yaml) [![Doxygen](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/docs.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/docs.yaml)

# soil-power-sensor-firmware

Firmware repository for STM32 and ESP32 microcontrollers on the Soil Power Sensor board along with supporting libraries.

## Project Structure

All components of the soil power sensor project are open source and live across a range of repositories. This repository serves as a central hub for all components for the project including software and hardware.

| Repository | Description |
| --- | --- |
| https://github.com/jlab-sensing/soil-power-sensor-firmware | Firmware for microcontrollers |
| https://github.com/jlab-sensing/platform-ststm32 | Fork with our board and modifications to build script |
| https://github.com/jlab-sensing/tool-openocd | For allowing for user defined `openocd` binary |
| https://github.com/jlab-sensing/STM32CubeWL | Platformio library for STMicroelectronics HAL and other libraries |
| https://github.com/jlab-sensing/soil_power_sensor | Hardware design files for the sensor board |

## File structure of repository

The following paths are the major parts of the project. Each individual folder has more information relating to implementation and development.

| Path | Description | Link |
| --- | --- | --- |
| `stm32/` | Platformio project for the stm32-based Wio-E5 mini | @ref ./stm32/README.md |
| `esp32/` | Platformio project for the ESP32-C3-MINI-N1 | @ref ./esp32/README.md |
| `proto/` | Protobuf definitions and language specific libraries | @ref ./proto/README.md |

## Software Requirements

The following is the list of the software used for developing the firmware. The versions listed were the ones used for developing the initial release of the firmware. Versions too far into the past might cause unforeseen errors.

| Software | Version | Optional |
| --- | --- | --- |
| [platformio](https://platformio.org/) | `6.1.7` | No |
| [stm32pio](https://github.com/ussserrr/stm32pio) | `2.1.0` | Yes, for code generation |
| [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) | `6.10.0` | Yes, for code generation |
| [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) | `1.13.2` | Yes, for flashing with openocd on Windows |
| [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html) | `2.15.0` | Yes, for initial flash and clearing read protection from Wio-E5 |
| [Nanopb](https://jpa.kapsi.fi/nanopb/) | `0.4.8` | No |
| [protobuf](https://protobuf.dev/) | `25.2` | No |
| Make | `4.4.1` | No |

## Generation documentation

This project use [Doxygen](https://www.doxygen.nl/) for its code documentation. HTML documentation is automatically generated through Github Actions and is updated whenever there is a change to the `main` branch. To generate documentation locally in the `docs/` folder, run the following from the root directory:

```bash
doxygen Doxyfile
```

Online version of the documentation for the `main` branch is available at https://jlab-sensing.github.io/soil-power-sensor-firmware/

## Resources

Below is a list of resources that served as references throughout the design and implementation for the project.

- https://github.com/Seeed-Studio/LoRaWan-E5-Node