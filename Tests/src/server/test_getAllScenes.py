from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# setMicLevel
request_dict = {
    "command": "getAllScenes",
}
request_body = json.dumps(request_dict)
request = request_body.encode()
print("\n--------------------------------------------")
print("getAllScenes : sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s, True)
time.sleep(2.0)
receive(s, True)
