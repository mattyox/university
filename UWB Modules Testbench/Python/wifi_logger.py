import socket
from datetime import datetime, timedelta
import argparse

def main(host, port, log_file):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((host, port))

    except socket.error as e:
        print(f"Socket error: {e}")
        exit(1)
    
    with open(log_file, "w") as lf:
        try:
            start_time = datetime.now()
            end_time = start_time + timedelta(minutes=10)

            print(f"Logging UDP packets from {host} at {port} to {log_file}...")

            while datetime.now() < end_time:
                data, addr = sock.recvfrom(1024)
                message = data.decode()
                if message:
                    now = datetime.now()
                    timestamp = now.strftime("[%H:%M:%S.") + f"{now.microsecond:06d}]"
                    output_line = f"{timestamp} {message}"
                    lf.write(output_line + "\n")
                    lf.flush()

        except Exception as e:
            print(f"UDP packet error: {e}")

        except KeyboardInterrupt:
            print("Exiting...")

        finally:
            print("Finished logging.")
            sock.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Wi-Fi logger with timestamps")
    parser.add_argument("-n", "--node", default="0.0.0.0", help="Node IP address (default: %(default)s)")
    parser.add_argument("-p", "--port", type=int, default="12345", help="Node port (default: %(default)s)")
    parser.add_argument("-f", "--file", default="log_file.txt", help="Output log file path (default: %(default)s)")
    
    args = parser.parse_args()
    main(args.node, args.port, args.file)
