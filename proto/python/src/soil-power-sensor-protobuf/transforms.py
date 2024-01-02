"""Binary data transforms

.. author:: John Madden <jmadden173@pm.me>
   :date: 2024-01-01
"""

from .soil_power_sensor_pb2 import Response, ResponseCode

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
        response.resp = ResponseCode.SUCCESS
    else:
        response.resp = ResponseCode.ERROR
       
    # return encode message 
    return response.SerializeToString()


def decode(data: bytes):
    """Decodes the measurement data
    """
    
    
