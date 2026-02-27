import serial
from datetime import datetime, timedelta
import argparse

def main(port, log_file):
    try:
        sp = serial.Serial(port, 115200, timeout=None)
        
    except serial.SerialException as expection:
        print(f"Error opening serial port {port}: {expection}")
        return
    
    with open(log_file, "w") as lf:
        try:
            start_time = datetime.now()
            end_time = start_time + timedelta(minutes=10)

            print(f"Logging from {port} at 115200 baud rate to {log_file}...")

            while datetime.now() < end_time:
                line = sp.readline().decode(errors='ignore').rstrip()
                if line:
                    now = datetime.now()
                    timestamp = now.strftime("[%H:%M:%S.") + f"{now.microsecond:06d}]"
                    output_line = f"{timestamp} {line}"
                    lf.write(output_line + "\n")
                    lf.flush()

        except Exception as e:
            print(f"Serial connection error: {e}")

        except KeyboardInterrupt:
            print("Exiting...")

        finally:
            print("Finished logging.")
            sp.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Serial logger with timestamps")
    # parser.add_argument("-p", "--port", default="/dev/tty.usbserial-02313124", help="Serial port (default: %(default)s)") # macOS
    parser.add_argument("-p", "--port", default="/dev/ttyUSB0", help="Serial port (default: %(default)s)") # BeagleBone Black and Raspberry Pi 4B
    parser.add_argument("-f", "--file", default="log_file.txt", help="Output log file path (default: %(default)s)")
    
    args = parser.parse_args()
    main(args.port, args.file)
