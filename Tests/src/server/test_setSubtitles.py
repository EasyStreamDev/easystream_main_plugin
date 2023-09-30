from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)

# setMicLevel
request_dict1 = {
    "command": "setSubtitles",
    "params": {
        "uuid": "7795b244-2a3c-48cc-a4f3-f08329c90c66",
        "length": 1,
        "linked_mics": ["Mic/Aux"],
    },
}

request_body = json.dumps(request_dict1)
request = request_body.encode()
print("\n--------------------------------------------")
print("setMicLevel : sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)


# request_dict2 = {
#     "command": "setSubtitles",
#     "params": {
#         "uuid": "c19df62b-7994-4398-a061-53d196d7eb5c",
#         "length": 1,
#         "linked_mics": ["Desktop Audio"],
#     },
# }

# request_body = json.dumps(request_dict2)
# request = request_body.encode()
# print("\n--------------------------------------------")
# print("setMicLevel : sending...")
# send(s, request)
# print("Sent ! Waiting for server response...")
# receive(s, True)
