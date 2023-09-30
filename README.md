# soil-power-sensor-firmware
Firmware for STM32 and ESP32 microcontrollers on the Soil Power Sensor board

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