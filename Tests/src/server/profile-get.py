from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)

try:
    # setMicLevel
    request_dict = {
        "command": "/profile/get",
    }
    request_body = json.dumps(request_dict)
    request = request_body.encode()
    print("\n--------------------------------------------")
    print("/profile/get: sending...")
    send(s, request)
    print("Sent ! Waiting for server response...")
    receive(s, True)
except:
    print("Response too long for python apparently.")
