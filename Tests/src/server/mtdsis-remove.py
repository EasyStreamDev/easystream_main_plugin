from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# setMicLevel
request_dicts = [
    {
        "command": "/mtdsis/remove",
        "params": {
            "mic_id": "d3486cdc-3ffb-4123-882c-f27150ac9f91",
        },
    },
    {
        "command": "/mtdsis/remove",
        "params": {
            "mic_id": "2d3384a3-f9ca-4c7e-9c5d-98bf949420d3",
        },
    },
]

for r in request_dicts:
    request_body = json.dumps(r)
    request = request_body.encode()
    print("\n--------------------------------------------")
    print("/mtdsis/remove : sending...")
    send(s, request)
    print("Sent ! Waiting for server response...")
    receive(s, True)
