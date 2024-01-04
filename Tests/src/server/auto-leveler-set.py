from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# setMicLevel
request_dicts = [
    {
        "command": "/microphones/auto-leveler/set",
        "params": {
            "micName": "Mic/Aux",
            "level": 0,
            "isActive": False,
        },
    },
    {
        "command": "/microphones/auto-leveler/set",
        "params": {
            "micName": "Desktop Audio",
            "level": 0,
            "isActive": False,
        },
    },
]

for r in request_dicts:
    request_body = json.dumps(r)
    request = request_body.encode()
    print("\n--------------------------------------------")
    print("setMicLevel : sending...")
    send(s, request)
    print("Sent ! Waiting for server response...")
    receive(s, True)
