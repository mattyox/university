import socket

SERVER_IP = "XXX.XXX.XXX.XXX"
SERVER_PORT = 1234
TIMEOUT = 3

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(TIMEOUT)

try:
    message = "Hello from UDP client"
    print(f"Sending: {message}")
    sock.sendto(message.encode(), (SERVER_IP, SERVER_PORT))

    data, addr = sock.recvfrom(2048)
    print(f"Received from server: {data.decode()}")

except socket.timeout:
    print("No response (timeout)")

finally:
    sock.close()
