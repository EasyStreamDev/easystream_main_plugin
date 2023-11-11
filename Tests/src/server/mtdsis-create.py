from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# setMicLevel
request_dicts = [
    {
        "command": "/mtdsis/create",
        "params": {
            "mic_id": "d3486cdc-3ffb-4123-882c-f27150ac9f91",  # Mic/Aux
            "display_sources_ids": ["eff55441-ba11-4dee-94d0-5ad8e727c929"],  # cam
        },
    },
    {
        "command": "/mtdsis/create",
        "params": {
            "mic_id": "2d3384a3-f9ca-4c7e-9c5d-98bf949420d3",  # Desktop
            "display_sources_ids": ["5292242b-21bf-4e41-bea8-70cf25cc8a79"],  # screen
        },
    },
]
for r in request_dicts:
    request_body = json.dumps(r)
    request = request_body.encode()
    print("\n--------------------------------------------")
    print("/mtdsis/create : sending...")
    send(s, request)
    print("Sent ! Waiting for server response...")
    receive(s, True)
