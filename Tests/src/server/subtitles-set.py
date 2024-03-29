from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)

request_dict1 = {
    "command": "/subtitles/set",
    "params": {
        "uuid": "7795b244-2a3c-48cc-a4f3-f08329c90c66",
        "length": 1,
        "linked_mics": ["Mic/Aux"],
    },
}

request_body = json.dumps(request_dict1)
request = request_body.encode()
print("\n--------------------------------------------")
print("/subtitles/set: sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)
