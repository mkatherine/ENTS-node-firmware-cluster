from google.protobuf import message
from google.protobuf.json_format import MessageToDict

from .soil_power_sensor_pb2 import Esp32Command, PageCommand

def encode_esp32command(cmd_type: str, **kwargs) -> bytes:
    """Encodes a command for the esp32
    
    Args:
        cmd: Type of command to encode
        **kwargs: See other encode functions
        
    Returns:
        Bytes of protobuf serialized message 
    """
    
    cmd = Esp32Command()
     
    if (cmd_type == "page"):
        #import pdb; pdb.set_trace()
        page_cmd = encode_page_command(**kwargs)
        cmd.page_command.CopyFrom(page_cmd)
    else:
        raise NotImplementedError(f"Command type {cmd_type} not implemented!")
    
    return cmd.SerializeToString()

def encode_page_command(
    req: str,
    fd: int,
    bs: int = 0,
    n: int = 0
) -> message:
    """Encodes a command for memory paging
   
    Args:
        req: Page request type
        fd: File descriptor
        bs: Block size 
        n: Number of bytes
        
    Return:
        PageCommand message 
        
    Raises:
        NotImplementedError: When page request type does not exist
    """
   
    # look up table to request types 
    req_lut = {
        "open": PageCommand.RequestType.OPEN,
        "close": PageCommand.RequestType.CLOSE,
        "read": PageCommand.RequestType.READ,
        "write": PageCommand.RequestType.WRITE,
    }
   
    # ensure lower case 
    req = req.lower()
    
    page_cmd = PageCommand()
    try:
        page_cmd.file_request = req_lut[req]
    except KeyError as exc:
        raise NotImplementedError(f"File request type {req} not implemented") from exc
    page_cmd.file_descriptor = fd
    page_cmd.block_size = bs
    page_cmd.num_bytes = n
    
    return page_cmd
    

def decode_esp32command(data: bytes) -> dict:
    cmd = Esp32Command()
   
    # parse serialized message
    cmd.ParseFromString(data)
    
    # return dicti
    return MessageToDict(cmd, always_print_fields_with_no_presence=True)