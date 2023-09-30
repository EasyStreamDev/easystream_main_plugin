from _base import *
from _base import test_socket as s

connect(s, HOST, PORT)

while 1:
    receive(s)
    time.sleep(2.0)
