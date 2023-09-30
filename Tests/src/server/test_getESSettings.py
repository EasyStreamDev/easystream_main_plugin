from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# setMicLevel
request_dict = {
    "command": "getProfileSettings",
}
request_body = json.dumps(request_dict)
request = request_body.encode()
print("\n--------------------------------------------")
print("getProfileSettings : sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)
