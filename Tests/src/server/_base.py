import socket
import time
import json

HOST = "0.0.0.0"
PORT = 47920  # The same port as used by the server


def send(s: socket.socket, data: bytes):
    s.sendall(data)


def receive(s: socket.socket, display: bool = False) -> dict:
    response = json.loads(s.recv(2048).decode())
    if display:
        print("Server response : ", end="")
        print(json.dumps(response, indent=4))
    return response


def connect(s: socket.socket, host: str, port: int):
    s.connect((host, port))
    receive(s)


test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
