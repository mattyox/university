import socket
from datetime import datetime
import argparse

def main(host, port):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((host, port))

    except socket.error as e:
        print(f"Socket error: {e}")
        exit(1)
    
    print(f"Monitoring UDP packets from {host} at {port} ...")

    try:
        while True:
            data, addr = sock.recvfrom(1024)
            message = data.decode()
            if message:
                now = datetime.now()
                timestamp = now.strftime("[%H:%M:%S.") + f"{now.microsecond:06d}]"
                print(f"{timestamp} {message}")

    except Exception as e:
        print(f"UDP packet error: {e}")

    except KeyboardInterrupt:
        print("Exiting...")

    finally:
        print("Finished logging.")
        sock.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Wi-Fi monitor with timestamps")
    parser.add_argument("-n", "--node", default="0.0.0.0", help="Node IP address (default: %(default)s)")
    parser.add_argument("-p", "--port", type=int, default="12345", help="Node port (default: %(default)s)")
    
    args = parser.parse_args()
    main(args.node, args.port)
