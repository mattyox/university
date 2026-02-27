import socket
import signal
import sys

HOST = "0.0.0.0"   # Listen on all interfaces
PORT = 124
BUFFER_SIZE = 2048

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

print(f"UDP server listening on {HOST}:{PORT}")

def shutdown(sig, frame):
    print("\nShutting down server...")
    sock.close()
    sys.exit(0)

signal.signal(signal.SIGINT, shutdown)

while True:
    try:
        data, addr = sock.recvfrom(BUFFER_SIZE)
        message = data.decode(errors="ignore")
        print(f"Received from {addr}: {message}")

        # Echo back
        response = f"ACK: {message}"
        sock.sendto(response.encode(), addr)

    except Exception as e:
        print(f"Error: {e}")
