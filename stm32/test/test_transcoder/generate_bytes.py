#!/usr/bin/env python

"""
Generates serialized protobuf data to be used in test_transcoder. The output can
be copied directly into the test files.

The `soil_power_sensor_protobuf` package needs to be installed first.

```bash
./generate_bytes.py
```
"""


from soil_power_sensor_protobuf import (encode_response,
                                        encode_power_measurement,
                                        encode_teros12_measurement)


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


if __name__ == "__main__":
    print("Power Bytes:")
    meas_power = encode_power_measurement(1436079600, 4, 7, 37.13, 185.29)
    print_bytes_c(meas_power)
    print()

    print("Teros12 Bytes")
    meas_teros12 = encode_teros12_measurement(1436079600, 4, 7, 2124.62, 0.43,
                                              24.8, 123)
    print_bytes_c(meas_teros12)
    print()

    print("Response Success Bytes")
    resp_success = encode_response(True)
    print_bytes_c(resp_success)
    print()

    print("Response Error Bytes")
    resp_fail = encode_response(False)
    print_bytes_c(resp_fail)
    print()
