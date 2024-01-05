#!/usr/bin/env python

"""
Generates serialized protobuf data to be used in test_transcoder. The output can
be copied directly into the test files.

The `soil_power_sensor_protobuf` package needs to be installed first.

```bash
./generate_bytes.py
```
"""

import pdb

from datetime import datetime

from soil_power_sensor_protobuf.soil_power_sensor_pb2 import Measurement, Response


def print_bytes_c(data : bytes) -> str:
    """Formats serialized data into c bytes array"""

    # format data string
    data_str = "uint8_t data[] = {"
    hex_str = [hex(d) for d in data]
    data_str += ", ".join(hex_str) 
    data_str += "};"

    # print data string
    print(data_str)
    
    # print length of data string
    print(f'size_t data_len = {len(hex_str)};')


def generate_power() -> bytes:
    meas = Measurement()

    # meta
    meas.meta.cell_id = 4
    meas.meta.logger_id = 7
    meas.meta.ts.FromDatetime(datetime(2015, 7, 5))

    # power
    meas.power.voltage = 37.13
    meas.power.current = 185.29

    # return serialized data
    return meas.SerializeToString()


def generate_teros12() -> bytes:
    meas = Measurement()

    # meta
    meas.meta.cell_id = 4
    meas.meta.logger_id = 7
    meas.meta.ts.FromDatetime(datetime(2015, 7, 5))

    # teros12
    meas.teros12.vwc_raw = 2124.62
    meas.teros12.vwc_adj = 0.43
    meas.teros12.temp = 24.8
    meas.teros12.ec = 123

    return meas.SerializeToString()


def generate_response_success() -> bytes:
    resp = Response()
    resp.resp = Response.ResponseType.SUCCESS
    return resp.SerializeToString()


def generate_response_error() -> bytes:
    resp = Response()
    resp.resp = Response.ResponseType.ERROR
    return resp.SerializeToString()


if __name__ == "__main__":
    print("Power Bytes:")
    print_bytes_c(generate_power())
    print()

    print("Teros12 Bytes")
    print_bytes_c(generate_teros12())
    print()

    print("Response Success Bytes")
    print_bytes_c(generate_response_success())
    print()

    print("Response Error Bytes")
    print_bytes_c(generate_response_error())
    print()
