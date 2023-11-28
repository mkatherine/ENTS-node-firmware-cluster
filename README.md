[![Build stm32](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/stm32.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/stm32.yaml) [![Build esp32](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/esp32.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/esp32.yaml) [![Doxygen](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/docs.yaml/badge.svg)](https://github.com/jlab-sensing/soil-power-sensor-firmware/actions/workflows/docs.yaml)

# soil-power-sensor-firmware

Firmware for STM32 and ESP32 microcontrollers on the Soil Power Sensor board

## Steps to load into the STM32 bootloader

The `Wio-E5 mini` board can be programed via *SWD* through the debug header `D1`. The following are instructions for connecting a `ST-Link` debugger to the bootloader.

1. Connect the `ST-Link` debugger to header `D1`
1. Power the device
2. Press `RST` button
3. Press `BOOT` button
4. Release `RST` button
5. Release `BOOT` button
6. Press connect on the TSM32CubeProgrammer interface.
	- If this is a new board, the stock firmware will be flashed. You will see the error message `Error: data read failed`. This is expected since the stock firmware has read protection.
	- Otherwise, you see the current device memory.
7. Verify the target information in the lower right hand corner.

## Process for setting up new stm32 project

1. Install the following software

| Software | Link |
| --- | --- |
| `platformio` | https://platformio.org/ |
| `stm32pio` | https://github.com/ussserrr/stm32pio |
| `stm32cubemx` | https://www.st.com/en/development-tools/stm32cubemx.html |

2. Open CubeMX and create new project. Do NOT generate the code, `stm32pio` will perform that step.

3. Save project to desired project directory and close

4. Initialize the `stm32pio` project

```bash
stm32pio init -b wioe5
```

Check paths to CubeMX (`cubemx_cmd`) and Java (`java_cmd`) in `stm32pio.ini`. It's recomended to set `cubemx_cmd = STM32CubeMX` and `java_cmd = None` and the CubeMX launcher in your path.

Follow any recomendations listed in the commands ouput

5. Create a combined platformio and CubeMX project

```bash
stm32pio new -b wioe5
```

In `platformio.ini` change `platform = atmelavr` to `platform = ststm32`

6. Build the project

```bash
pio run
```


## Generating documentation

HTML documentation is generated with `doxygen` per project. Running the following command in `stm32/` and `esp32/` results in docs being generated in `stm32/docs/html/` and `stm32/docs/html` respectively.

```bash
doxygen Doxyfile
```

Online versions of the documentation is available for the main branch at

| Project | URL                                                  |
|---------|------------------------------------------------------|
| stm32   | https://jlab-sensing.github.io/soil-power-sensor-firmware/stm32/ |
| esp32   | https://jlab-sensing.github.io/soil-power-sensor-firmware/esp32/ |
