from google.protobuf import message
from google.protobuf.json_format import MessageToDict

from .soil_power_sensor_pb2 import Esp32Command, PageCommand, TestCommand

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
        page_cmd = encode_page_command(**kwargs)
        cmd.page_command.CopyFrom(page_cmd)
    elif (cmd_type == "test"):
        test_cmd = encode_test_command(**kwargs)
        cmd.test_command.CopyFrom(test_cmd)
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

def encode_test_command(state : str, data : int) -> message:
    """Encodes a command for testing modules library
    
    The following is of valid strings for state:
        "receive": Data is received by the module
        "receive_request": Data is received by the module indicating data for
                           subsequent request 
        "request": Data is sent my the module

    Args:
        state (str): State command string
        data (int): Integer to test passing of data
        
    Return:
        TestCommand message
        
    Raises:
        NotImplementedError: When the ChangeState type does not exist
    """
   
    # look up table for state strings 
    cs_lut = {
        "receive": TestCommand.ChangeState.RECEIVE,
        "receive_request": TestCommand.ChangeState.RECEIVE_REQUEST,
        "request": TestCommand.ChangeState.REQUEST,
    }
    
    # ensure all lower case
    state = state.lower() 
    
    test_cmd = TestCommand()
    try:
        test_cmd.state = cs_lut[state]
    except KeyError as exc:
        raise NotImplementedError(f"State command {state} not implemented") from exc
    test_cmd.data = data
    
    return test_cmd

def decode_esp32command(data: bytes) -> dict:
    cmd = Esp32Command()
   
    # parse serialized message
    cmd.ParseFromString(data)
    
    # return dicti
    return MessageToDict(cmd, always_print_fields_with_no_presence=True)