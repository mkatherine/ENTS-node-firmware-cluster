# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). The major and
minor versions follow hardware versioning with patches used to iterate on
firmware versions for a hardware version.

## [Unreleased]

### Added

- Contributing resources ([Contributing](./CONTRIBUTING.md),
  [Code ofConduct](./CODE_OF_CONDUCT.md), and Changelog)
- Issue and PR templates
- RTC time synchronization support through LoRaWAN [#48](i48)

[i48]: https://github.com/jlab-sensing/soil-power-sensor-firmware/issues/48

## [2.1.0] - 2024-02-25

Initial functional release of firmware. The firmware can preform the following:

- Measure voltage
- Read measurement from TEROS-12 over SDI-12
- Transmit measurement to Dirtviz through LoRaWAN