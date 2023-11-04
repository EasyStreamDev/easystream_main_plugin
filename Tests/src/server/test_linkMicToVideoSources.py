from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)

# setMicLevel
request_dict1 = {
    "command": "linkMicToDisplaySources",
    "params": {
        "mic_id": "",
        "display_sources_ids": [],
    },
}

request_body = json.dumps(request_dict1)
request = request_body.encode()
print("\n--------------------------------------------")
print("linkMicToDisplaySources : sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)
