"""Tests encoding and decoding of protobuf serialized data

Encoding for Response messages checks both a SUCCESS and ERROR can be obtained.
Decoding is performed to ensure data is preserved.

Decoding checks that the Measurement message is preserved through an
encoding/decoding cycle. Checks that missing fields result in an error and the
correct dictionary format is returned.
"""

import unittest

from soil_power_sensor_protobuf import (
    encode_response,
    decode_measurement,
    encode_esp32command,
    decode_esp32command,
)
from soil_power_sensor_protobuf.soil_power_sensor_pb2 import (
    Measurement,
    Response,
    MeasurementMetadata,
    Esp32Command,
)

from soil_power_sensor_protobuf.esp32 import PageCommand


class TestEncode(unittest.TestCase):
    def test_success(self):
        # encode
        resp_str = encode_response(success=True)

        # decode
        resp_out = Response()
        resp_out.ParseFromString(resp_str)

        self.assertEqual(Response.ResponseType.SUCCESS, resp_out.resp)

    def test_error(self):
        # encode
        resp_str = encode_response(success=False)

        # decode
        resp_out = Response()
        resp_out.ParseFromString(resp_str)

        self.assertEqual(Response.ResponseType.ERROR, resp_out.resp)


class TestDecode(unittest.TestCase):
    """Test decoding of measurements"""

    def setUp(self):
        """Creates a default metadata message"""
        self.meta = MeasurementMetadata()
        self.meta.ts = 1436079600
        self.meta.cell_id = 20
        self.meta.logger_id = 4

    def check_meta(self, meas_dict: dict):
        """Checks the measurement dictionary contains metadata information"""

        self.assertEqual(1436079600, meas_dict["ts"])
        self.assertEqual(20, meas_dict["cellId"])
        self.assertEqual(4, meas_dict["loggerId"])

    def test_power(self):
        """Test decoding of PowerMeasurement"""

        # import pdb; pdb.set_trace()
        # format measurement
        meas = Measurement()
        meas.meta.CopyFrom(self.meta)
        meas.power.voltage = 122.38
        meas.power.current = 514.81

        # serialize
        meas_str = meas.SerializeToString()

        # decode
        meas_dict = decode_measurement(data=meas_str)

        # check resulting dict
        self.assertEqual("power", meas_dict["type"])
        self.check_meta(meas_dict)
        self.assertAlmostEqual(122.38, meas_dict["data"]["voltage"])
        self.assertEqual(float, meas_dict["data_type"]["voltage"])
        self.assertAlmostEqual(514.81, meas_dict["data"]["current"])
        self.assertEqual(float, meas_dict["data_type"]["current"])

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
        meas_dict = decode_measurement(data=meas_str)

        # check dict
        self.assertEqual("teros12", meas_dict["type"])
        self.check_meta(meas_dict)
        self.assertAlmostEqual(2124.62, meas_dict["data"]["vwcRaw"])
        self.assertEqual(float, meas_dict["data_type"]["vwcRaw"])
        self.assertAlmostEqual(0.43, meas_dict["data"]["vwcAdj"])
        self.assertEqual(float, meas_dict["data_type"]["vwcAdj"])
        self.assertAlmostEqual(24.8, meas_dict["data"]["temp"])
        self.assertEqual(float, meas_dict["data_type"]["temp"])
        self.assertEqual(123, meas_dict["data"]["ec"])
        self.assertEqual(int, meas_dict["data_type"]["ec"])

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
            decode_measurement(data=meas_str)

    def test_missing_measurement(self):
        """Test that error is raised when measurement is missing"""

        # format measurement
        meas = Measurement()
        meas.meta.CopyFrom(self.meta)

        # serialize
        meas_str = meas.SerializeToString()

        # decode
        with self.assertRaises(KeyError):
            decode_measurement(data=meas_str)

class TestEsp32(unittest.TestCase):
    def test_cmd_not_implemented(self):
        """Checks that an exception is raised when a non-existing command is
        called"""
        
        with self.assertRaises(NotImplementedError):
            encode_esp32command("agg", req="open", fd=123, bs=456, n=789)
    
    def test_page_encode(self):
        """Test encoding a page command"""
        
        req = "open"
        fd = 1
        bs = 512
        n = 1024
        
        cmd_str = encode_esp32command("page", req=req, fd=fd, bs=bs, n=n)
        
        cmd = Esp32Command()
        cmd.ParseFromString(cmd_str)
       
        # check the command type 
        cmd_type = cmd.WhichOneof("command")
        self.assertEqual(cmd_type, "PageCommand")
      
        # check individual values
        self.assertEqual(cmd.page_command.file_request, PageCommand.RequestType.OPEN)
        self.assertEqual(cmd.page_command.file_descriptor, fd)
        self.assertEqual(cmd.page_command.block_size, bs)
        self.assertEqual(cmd.page_command.num_bytes, n)

    def test_page_decode(self):
        """Test decoding a page command"""
        
        req = "open"
        fd = 1
        bs = 512
        n = 1024
        
        cmd_str = encode_esp32command("page", req=req, fd=fd, bs=bs, n=n)
      
        cmd = decode_esp32command(cmd_str)
      
        # check individual values
        self.assertEqual(cmd["file_request"], PageCommand.RequestType.OPEN)
        self.assertEqual(cmd["file_descriptor"], fd)
        self.assertEqual(cmd["block_size"], bs)
        self.assertEqual(cmd["num_bytes"], n)

    def test_page_req_not_implemented(self):
        """Test encoding a page command with a not implemented request"""
        
        with self.assertRaises(NotImplementedError):
            encode_esp32command("page", req="agg", fd=123, bs=456, n=789)

if __name__ == "__main__":
    unittest.main()
