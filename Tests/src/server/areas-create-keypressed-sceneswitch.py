from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# setMicLevel
request_dict = {
    "command": "/areas/create",
    "params": {
        "action": {
            "type": "KEY_PRESSED",
            "params": {
                "key": "c",
            },
        },
        "reaction": {
            "name": "area1",
            "type": "SCENE_SWITCH",
            "params": {
                "uuid": "ec40025d-6c17-4199-afaf-28c1870f931a",  # valid UUID
                # "uuid": "7b7df3b-6155-4582-9d4e-9500057a7851", # invalid UUID
            },
        },
    },
}
request_body = json.dumps(request_dict)
request = request_body.encode()
print("\n--------------------------------------------")
print("/areas/create: sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)
