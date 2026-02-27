#!/usr/bin/env python3

import socket

UDP_IP = "ot-host.local"
UDP_PORT = 54321
MESSAGE = "Hello, World!"
BUFFER_SIZE = 1024

# print("UDP target IP:", UDP_IP)
# print("UDP target port:", UDP_PORT)
# print("message:", MESSAGE)

sock = socket.socket(socket.AF_INET6, # Internet
                        socket.SOCK_DGRAM) # UDP
# sock.sendto(MESSAGE.encode("UTF-8"), (UDP_IP, UDP_PORT))
sock.sendto(bytearray(8), (UDP_IP, UDP_PORT))
 
msg_from_server = sock.recvfrom(BUFFER_SIZE)

# msg = "Message from Server {}".format(msg_from_server[0])

print(msg_from_server[0])