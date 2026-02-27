import serial
from datetime import datetime
import argparse

def main(port):
    try:
        sp = serial.Serial(port, 115200, timeout=None)
        
    except serial.SerialException as expection:
        print(f"Error opening serial port {port}: {expection}")
        return
    
    print(f"Monitoring from {port} at 115200 baud rate...")
    
    try:
        while True:
            line = sp.readline().decode(errors='ignore').rstrip()
            now = datetime.now()
            timestamp = now.strftime("[%H:%M:%S.") + f"{now.microsecond:06d}]"
            print(f"{timestamp} {line}")

    except Exception as e:
        print(f"Serial connection error: {e}")

    except KeyboardInterrupt:
        print("Exiting...")

    finally:
        print("Finished logging.")
        sp.close()
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Serial monitor with timestamps")
    # parser.add_argument("-p", "--port", default="/dev/tty.usbserial-02313124", help="Serial port (default: %(default)s)") # macOS
    parser.add_argument("-p", "--port", default="/dev/ttyUSB0", help="Serial port (default: %(default)s)") # BeagleBone Black and Raspberry Pi 4B
    
    args = parser.parse_args()
    main(args.port)
