from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)

print("connected")
# setMicLevel
request_dict = {
    "command": "/areas/get",
}
request_body = json.dumps(request_dict)
request = request_body.encode()
print("\n--------------------------------------------")
print("/areas/get: sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)
