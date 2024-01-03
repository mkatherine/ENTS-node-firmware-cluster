"""Binary data transforms

.. author:: John Madden <jmadden173@pm.me>
   :date: 2024-01-01
"""

from google.protobuf.json_format import MessageToDict

from .soil_power_sensor_pb2 import Measurement, Response

def encode(success: bool = True):
    """Encodes a response
    
    The response indicates there was a successful upload, otherwise indicates an
    error.
    
    Args:
        success: Specifies whether to encode a success or failure.
        
    Returns:
        Serialized response message
    
    """
   
    # format response 
    response = Response()
    if success:
        response.resp = Response.ResponseType.SUCCESS
    else:
        response.resp = Response.ResponseType.ERROR
       
    # return encode message 
    return response.SerializeToString()


def decode(data: str):
    """Decodes the measurement data
    
    Uses protobuf to decode the serialized data and transform the measurement
    into a dictionary.
    
    Args:
        data: String of protobuf serialized data from the Measurement message
    
    Returns:
        Flat dictionary of values from the meta field, measurement field, and
        the key "type" to indicate the type of measurement
        
    Raises:
        KeyError: When the serialized data is missing a requires field
    """

    # parse data
    meas = Measurement()
    meas.ParseFromString(data)
    
    # convert meta into dict
    if not meas.HasField("meta"):
        raise KeyError("Measurement missing metadata")
    meta_dict = MessageToDict(meas.meta)
    
    # decode measurement
    if not meas.HasField("measurement"):
        raise KeyError("Measurement missing data")
    measurement_type = meas.WhichOneof("measurement")
    measurement_dict = MessageToDict(getattr(meas, measurement_type))
    measurement_dict["type"] = measurement_type

    # merge dictionaries
    meas_dict = {**meta_dict, **measurement_dict}
    
    return meas_dict 