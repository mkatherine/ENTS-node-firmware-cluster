# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: soil_power_sensor.proto
# Protobuf Python Version: 5.29.2
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    29,
    2,
    '',
    'soil_power_sensor.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x17soil_power_sensor.proto\"E\n\x13MeasurementMetadata\x12\x0f\n\x07\x63\x65ll_id\x18\x01 \x01(\r\x12\x11\n\tlogger_id\x18\x02 \x01(\r\x12\n\n\x02ts\x18\x03 \x01(\r\"4\n\x10PowerMeasurement\x12\x0f\n\x07voltage\x18\x02 \x01(\x01\x12\x0f\n\x07\x63urrent\x18\x03 \x01(\x01\"P\n\x12Teros12Measurement\x12\x0f\n\x07vwc_raw\x18\x02 \x01(\x01\x12\x0f\n\x07vwc_adj\x18\x03 \x01(\x01\x12\x0c\n\x04temp\x18\x04 \x01(\x01\x12\n\n\x02\x65\x63\x18\x05 \x01(\r\"6\n\x12Teros21Measurement\x12\x12\n\nmatric_pot\x18\x01 \x01(\x01\x12\x0c\n\x04temp\x18\x02 \x01(\x01\"<\n\x13Phytos31Measurement\x12\x0f\n\x07voltage\x18\x01 \x01(\x01\x12\x14\n\x0cleaf_wetness\x18\x02 \x01(\x01\"L\n\x11\x42ME280Measurement\x12\x10\n\x08pressure\x18\x01 \x01(\r\x12\x13\n\x0btemperature\x18\x02 \x01(\x05\x12\x10\n\x08humidity\x18\x03 \x01(\r\"\x84\x02\n\x0bMeasurement\x12\"\n\x04meta\x18\x01 \x01(\x0b\x32\x14.MeasurementMetadata\x12\"\n\x05power\x18\x02 \x01(\x0b\x32\x11.PowerMeasurementH\x00\x12&\n\x07teros12\x18\x03 \x01(\x0b\x32\x13.Teros12MeasurementH\x00\x12(\n\x08phytos31\x18\x04 \x01(\x0b\x32\x14.Phytos31MeasurementH\x00\x12$\n\x06\x62me280\x18\x05 \x01(\x0b\x32\x12.BME280MeasurementH\x00\x12&\n\x07teros21\x18\x06 \x01(\x0b\x32\x13.Teros21MeasurementH\x00\x42\r\n\x0bmeasurement\"X\n\x08Response\x12$\n\x04resp\x18\x01 \x01(\x0e\x32\x16.Response.ResponseType\"&\n\x0cResponseType\x12\x0b\n\x07SUCCESS\x10\x00\x12\t\n\x05\x45RROR\x10\x01\"\xdc\x02\n\x11UserConfiguration\x12\x11\n\tlogger_id\x18\x01 \x01(\r\x12\x0f\n\x07\x63\x65ll_id\x18\x02 \x01(\r\x12$\n\rUpload_method\x18\x03 \x01(\x0e\x32\r.Uploadmethod\x12\x17\n\x0fUpload_interval\x18\x04 \x01(\r\x12\'\n\x0f\x65nabled_sensors\x18\x05 \x03(\x0e\x32\x0e.EnabledSensor\x12\x15\n\rVoltage_Slope\x18\x06 \x01(\x01\x12\x16\n\x0eVoltage_Offset\x18\x07 \x01(\x01\x12\x15\n\rCurrent_Slope\x18\x08 \x01(\x01\x12\x16\n\x0e\x43urrent_Offset\x18\t \x01(\x01\x12\x11\n\tWiFi_SSID\x18\n \x01(\t\x12\x15\n\rWiFi_Password\x18\x0b \x01(\t\x12\x18\n\x10\x41PI_Endpoint_URL\x18\x0c \x01(\t\x12\x19\n\x11\x41PI_Endpoint_Port\x18\r \x01(\r*O\n\rEnabledSensor\x12\x0b\n\x07Voltage\x10\x00\x12\x0b\n\x07\x43urrent\x10\x01\x12\x0b\n\x07Teros12\x10\x02\x12\x0b\n\x07Teros21\x10\x03\x12\n\n\x06\x42ME280\x10\x04*\"\n\x0cUploadmethod\x12\x08\n\x04LoRa\x10\x00\x12\x08\n\x04WiFi\x10\x01\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'soil_power_sensor_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_ENABLEDSENSOR']._serialized_start=1134
  _globals['_ENABLEDSENSOR']._serialized_end=1213
  _globals['_UPLOADMETHOD']._serialized_start=1215
  _globals['_UPLOADMETHOD']._serialized_end=1249
  _globals['_MEASUREMENTMETADATA']._serialized_start=27
  _globals['_MEASUREMENTMETADATA']._serialized_end=96
  _globals['_POWERMEASUREMENT']._serialized_start=98
  _globals['_POWERMEASUREMENT']._serialized_end=150
  _globals['_TEROS12MEASUREMENT']._serialized_start=152
  _globals['_TEROS12MEASUREMENT']._serialized_end=232
  _globals['_TEROS21MEASUREMENT']._serialized_start=234
  _globals['_TEROS21MEASUREMENT']._serialized_end=288
  _globals['_PHYTOS31MEASUREMENT']._serialized_start=290
  _globals['_PHYTOS31MEASUREMENT']._serialized_end=350
  _globals['_BME280MEASUREMENT']._serialized_start=352
  _globals['_BME280MEASUREMENT']._serialized_end=428
  _globals['_MEASUREMENT']._serialized_start=431
  _globals['_MEASUREMENT']._serialized_end=691
  _globals['_RESPONSE']._serialized_start=693
  _globals['_RESPONSE']._serialized_end=781
  _globals['_RESPONSE_RESPONSETYPE']._serialized_start=743
  _globals['_RESPONSE_RESPONSETYPE']._serialized_end=781
  _globals['_USERCONFIGURATION']._serialized_start=784
  _globals['_USERCONFIGURATION']._serialized_end=1132
# @@protoc_insertion_point(module_scope)
