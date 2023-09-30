from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)


# Sub to broadcast
request = json.dumps(
    {
        "command": "subscribeBroadcast",
        "params": {
            "enable": True,
        },
    }
).encode()

print("\n--------------------------------------------")
print("subscribeBroadcast : sending...")
send(s, request)

print("Sent ! Waiting for server response...")
response: dict = receive(s)
if int(response.get("statusCode")) == 200:
    print("Succesfully subscribed to broadcast!")

# Display any broadcasting request
try:
    while 1:
        receive(s, display=True)
        time.sleep(0.5)
except:
    pass
