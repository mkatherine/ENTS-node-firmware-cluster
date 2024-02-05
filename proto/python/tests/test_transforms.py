"""Tests encoding and decoding of protobuf serialized data

Encoding of both a SUCCESS and ERROR is tested. Decoding is performed to ensure
data is preserved.

Decoding checks that data in the message is preserved through an
encoding/decoding cycle. Checks that missing fields result in an error and the
correct dictionary format is returned.
"""

import unittest
from datetime import datetime

from soil_power_sensor_protobuf import encode, decode
from soil_power_sensor_protobuf.soil_power_sensor_pb2 import Measurement, Response, PowerMeasurement, Teros12Measurement, MeasurementMetadata
from soil_power_sensor_protobuf.timestamp_pb2 import Timestamp


class TestEncode(unittest.TestCase):
    def test_success(self):
        # encode
        resp_str = encode(success=True)

        # decode
        resp_out = Response()
        resp_out.ParseFromString(resp_str)

        self.assertEqual(Response.ResponseType.SUCCESS, resp_out.resp)

    def test_error(self):
        # encode
        resp_str = encode(success=False)

        # decode
        resp_out = Response()
        resp_out.ParseFromString(resp_str)

        self.assertEqual(Response.ResponseType.ERROR, resp_out.resp)


class TestDecode(unittest.TestCase):
    """Test decoding of measurements"""

    def setUp(self):
        """Creates a default metadata message"""
        ts = Timestamp()
        ts.FromDatetime(datetime(2013, 6, 2))

        self.meta = MeasurementMetadata()
        self.meta.ts.CopyFrom(ts)
        self.meta.cell_id = 20
        self.meta.logger_id = 4

    def check_meta(self, meas_dict: dict):
        """Checks the measurement dictionary contains metadata information"""

        self.assertEqual('2013-06-02T00:00:00Z', meas_dict["ts"])
        self.assertEqual(20, meas_dict["cellId"])
        self.assertEqual(4, meas_dict["loggerId"])

    def test_power(self):
        """Test decoding of PowerMeasurement"""

        # format measurement
        meas = Measurement()
        meas.meta.CopyFrom(self.meta)
        meas.power.voltage = 122.38
        meas.power.current = 514.81

        # serialize
        meas_str = meas.SerializeToString()

        # decode
        meas_dict = decode(data=meas_str)

        # check resulting dict
        self.assertEqual("power", meas_dict["type"])
        self.check_meta(meas_dict)
        self.assertAlmostEqual(122.38, meas_dict["voltage"])
        self.assertAlmostEqual(514.81, meas_dict["current"])

    def test_teros12(self):
        """Test decoding of Teros12Measurement"""

        # format measurement
        meas = Measurement()
        meas.meta.CopyFrom(self.meta)
        meas.teros12.vwc_raw = 2124.62
        meas.teros12.vwc_adj = 0.43
        meas.teros12.temp = 24.8
        meas.teros12.ec = 123

        # serialize
        meas_str = meas.SerializeToString()

        # decode
        meas_dict = decode(data=meas_str)

        # check dict
        self.assertEqual("teros12", meas_dict["type"])
        self.check_meta(meas_dict)
        self.assertAlmostEqual(2124.62, meas_dict["vwcRaw"])
        self.assertAlmostEqual(0.43, meas_dict["vwcAdj"])
        self.assertAlmostEqual(24.8, meas_dict["temp"])
        self.assertEqual(123, meas_dict["ec"])

    def test_missing_meta(self):
        """Test that error is raised when meta is not set"""

        # format measurement
        meas = Measurement()
        meas.power.voltage = 122.38
        meas.power.current = 514.81

        # serialize
        meas_str = meas.SerializeToString()

        # decode
        with self.assertRaises(KeyError):
            decode(data=meas_str)

    def test_missing_measurement(self):
        """Test that error is raised when measurement is missing"""

        # format measurement
        meas = Measurement()
        meas.meta.CopyFrom(self.meta)

        # serialize
        meas_str = meas.SerializeToString()

        # decode
        with self.assertRaises(KeyError):
            decode(data=meas_str)


if __name__ == "__main__":
    unittest.main()
