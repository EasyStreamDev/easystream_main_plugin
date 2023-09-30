import socket
import time
import json

HOST = "0.0.0.0"
PORT = 47921  # The same port as used by the server


def send(s: socket.socket, data: bytes):
    s.sendall(data)


def receive(s: socket.socket):
    response = json.loads(s.recv(2048).decode())
    print("Server response : ", end="")
    print(json.dumps(response, indent=4))


def connect(s: socket.socket, host: str, port: int):
    s.connect((host, port))
    # receive(s)


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

###########
# CONNECT #
###########

print("Connecting...")
connect(s, HOST, PORT)
time.sleep(3.0)

################
# SET REQUESTS #
################

# setAutoAudioLeveler
# request_dict = {
#     "command": "setAutoAudioLeveler",
#     "params": {
#         "enable": True,
#         "source": "Mic/Aux",
#     },
# }
# request_body = json.dumps(request_dict)
# request = request_body.encode()
# print("\n--------------------------------------------")
# print("setAutoAudioLeveler : sending...")
# send(s, request)
# print("Sent ! Waiting for server response...")
# receive(s)
# time.sleep(2.0)


{
    # setMicLevel
    # request_dict = {
    #     "command": "setMicLevel",
    #     "params": {
    #         "name": "Mic/Aux",
    #         "level": 50,
    #         "setActive": True,
    #     },
    # }
    # request_body = json.dumps(request_dict)
    # request = request_body.encode()
    # print("\n--------------------------------------------")
    # print("setMicLevel : sending...")
    # send(s, request)
    # print("Sent ! Waiting for server response...")
    # receive(s)
    # time.sleep(2.0)
}

# setActionReaction
# request_dict = {
#     "command": "setActionReaction",
#     "params": {
#         "action": {
#             "type": "WORD_DETECT",
#             "params": {
#                 "words": ["juice"],
#             },
#         },
#         "reaction": {
#             "name": "ActionTEST name : tonti",
#             "type": "SCENE_SWITCH",
#             "params": {
#                 "scene": "TestScene1",
#             },
#         },
#     },
# }
# request_body = json.dumps(request_dict)
# request = request_body.encode()
# print("\n--------------------------------------------")
# print("setActionReaction : sending...")
# send(s, request)
# print("Sent ! Waiting for server response...")
# receive(s)
# time.sleep(2.0)


###################
# UPDATE REQUESTS #
###################

# updateAction
# request_dict = {
#     "command": "updateAction",
#     "params": {
#         "actionId": 1,
#         "type": "WORD_DETECT",
#         "params": {"words": ["floorboards"]},
#     },
# }
# request_body = json.dumps(request_dict)
# request = request_body.encode()
# print("\n--------------------------------------------")
# print("updateAction : sending...")
# send(s, request)
# print("Sent ! Waiting for server response...")
# receive(s)
# time.sleep(1.0)

# updateReaction
# request_dict = {
#     "command": "updateReaction",
#     "params": {
#         "name": "",
#         "reactionId": 1,
#         "type": "SCENE_SWITCH",
#         "params": {"scene": "TestScene2"},
#     },
# }
# request_body = json.dumps(request_dict)
# request = request_body.encode()
# print("\n--------------------------------------------")
# print("updateReaction : sending...")
# send(s, request)
# print("Sent ! Waiting for server response...")
# receive(s)


################
# GET REQUESTS #
################

# getAllMics
request_dict = {"command": "getAllMics"}
request_body = json.dumps(request_dict)
request = request_body.encode()
print("\n--------------------------------------------")
print("getAllMics : sending...")
send(s, request)
print("Sent ! Waiting for server response...")
receive(s)
# time.sleep(2.0)

# getActReactCouples
# request_dict = {"command": "getActReactCouples"}
# request_body = json.dumps(request_dict)
# request = request_body.encode()
# print("\n--------------------------------------------")
# print("getActReactCouples : sending...")
# send(s, request)
# print("Sent ! Waiting for server response...")
# receive(s)
# time.sleep(1.0)

###########
# CLOSING #
###########

s.close()
