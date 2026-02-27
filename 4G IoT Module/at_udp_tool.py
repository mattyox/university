#!/usr/bin/env python3

import serial
import threading
import queue
import time
import sys
import re
from datetime import datetime

# Config
UDP_LOG_FILE = "udp.log"

# Exceptions
class ATError(Exception):
    pass

# Logging
def log_udp(direction, peer, text):
    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    line = f"[{ts}] {direction:<8} {peer} | {text}\n"
    with open(UDP_LOG_FILE, "a", encoding="utf-8") as f:
        f.write(line)

# AT Modem Driver
class ATModem:
    def __init__(self, port, baudrate=115200):
        self.ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            timeout=0.1,
            write_timeout=1,
        )

        self.rx_queue = queue.Queue()
        self.udp_queue = queue.Queue()  # Incoming UDP indexes
        self.alive = True

        self.cmd_lock = threading.Lock()

        # Threads
        self.rx_thread = threading.Thread(target=self._reader, daemon=True)
        self.rx_thread.start()

        self.udp_worker = threading.Thread(target=self._udp_worker, daemon=True)
        self.udp_worker.start()

    # Reader thread
    def _reader(self):
        buffer = b""
        while self.alive:
            try:
                data = self.ser.read(256)
                if not data:
                    continue
                # print(f"[RAW] {data!r}")
                buffer += data

                # Detect '>' prompt anywhere
                while b">" in buffer:
                    before, buffer = buffer.split(b">", 1)
                    if before:
                        # Push any lines before >
                        for line_bytes in before.split(b"\r\n"):
                            line = line_bytes.decode(errors="ignore").strip()
                            if line:
                                print(f"[RX {time.strftime('%H:%M:%S')}] {line}")
                                if self._is_urc(line):
                                    self.handle_urc(line)
                                else:
                                    self.rx_queue.put(line)
                    print(f"[RX {time.strftime('%H:%M:%S')}] >")
                    self.rx_queue.put(">")

                # Push remaining CRLF-delimited lines
                while b"\r\n" in buffer:
                    line_bytes, buffer = buffer.split(b"\r\n", 1)
                    line = line_bytes.decode(errors="ignore").strip()
                    if not line:
                        continue
                    print(f"[RX {time.strftime('%H:%M:%S')}] {line}")
                    if self._is_urc(line):
                        self.handle_urc(line)
                    else:
                        self.rx_queue.put(line)

            except Exception as e:
                print(f"[RX ERROR] {e}")
                break

    # Core AT command
    def command(self, cmd, timeout=10):
        with self.cmd_lock:
            self._flush_rx()
            print(f"[TX {time.strftime('%H:%M:%S')}] {cmd}")
            self.ser.write((cmd + "\r").encode())

            lines = []
            t0 = time.time()
            while time.time() - t0 < timeout:
                try:
                    line = self.rx_queue.get(timeout=0.2)
                except queue.Empty:
                    continue

                if line == "OK":
                    return lines
                if line.startswith("ERROR") or line.startswith("+CME ERROR"):
                    raise ATError(line)

                lines.append(line)

            raise ATError(f"Timeout waiting for response to: {cmd}")

    # URC detection
    def _is_urc(self, line):
        return (
            line.startswith("+CEREG")
            or line.startswith("+CREG")
            or line.startswith("+CSCON")
            or line.startswith("NO CARRIER")
            or line.startswith("#MQEV")
            or line.startswith("+CMTI")
        )

    # URC handler
    def handle_urc(self, line):
        print(f"[URC] {line}")

        if line.startswith("+CMTI"):
            try:
                index = int(line.split(",")[1])
                self.udp_queue.put(index)
            except Exception as e:
                # ???
                print(f"[SMS ERROR] {e}")

    # UDP handling
    def init_udp(self):
        # ???
        return

    def send_udp(self, timeout=30):
        with self.cmd_lock:
            self._flush_rx()
            self.ser.write('AT#SD=1,1,XXX,"XXX.XXX.XXX.XXX",0,XXX,1\r'.encode())
            time.sleep(5)
            data = "Test"
            self.ser.write("AT#SSEND=1\r".encode())
            time.sleep(5)

            # Wait for '>' prompt
            t0 = time.time()
            got_prompt = False
            while time.time() - t0 < timeout:
                try:
                    line = self.rx_queue.get(timeout=0.5)
                except queue.Empty:
                    continue
                if line == ">":
                    got_prompt = True
                    break

            if not got_prompt:
                raise ATError("No UDP prompt")

            time.sleep(0.05)    # Slight delay before sending text

            # Send text + Ctrl-Z
            self.ser.write(data.encode())
            self.ser.write(b"\x1A")


            # ???
            # Wait for +CMGS and OK
            # t0 = time.time()
            # while time.time() - t0 < timeout:
            #     try:
            #         line = self.rx_queue.get(timeout=0.5)
            #     except queue.Empty:
            #         continue
            #     if line.startswith("+CMGS"):
            #         continue
            #     if line == "OK":
            #         print("UDP sent")
            #         # log_udp("SENT", f"To:{device_id}", data)
            #         return
            #     if "ERROR" in line:
            #         raise ATError(line)

            # raise ATError("Timeout waiting for SMS send completion")

    # ???
    def read_udp(self, index):
        lines = self.command(f"AT+CMGR={index}")

        header = lines[0]
        data = lines[1] if len(lines) > 1 else ""

        status_match = re.search(r'"(REC [A-Z]+)"', header)
        number_match = re.search(r'"(\+?[0-9]+)"', header)

        status = status_match.group(1) if status_match else "UNKNOWN"
        device_id = number_match.group(1) if number_match else "UNKNOWN"

        print("\n--- UDP RECEIVED ---")
        print(f"From: {device_id}")
        print(data)
        print("--------------------\n")

        return status, device_id, data

    # UDP worker (reads and logs)
    def _udp_worker(self):
        while self.alive:
            try:
                index = self.udp_queue.get(timeout=1)

            except queue.Empty:
                continue

            try:
                _, device_id, data = self.read_udp(index)
                log_udp("RECEIVED", f"From:{device_id}", data)

            except Exception as e:
                print(f"[UDP ERROR] {e}")

    # Utilities
    def _flush_rx(self):
        while not self.rx_queue.empty():
            try:
                self.rx_queue.get_nowait()
            except queue.Empty:
                break

    def close(self):
        self.alive = False
        self.rx_thread.join(timeout=1)
        self.udp_worker.join(timeout=1)
        self.ser.close()

# Interactive REPL
def repl(modem):
    print("\nAT and UDP Test Tool")
    print("Type AT commands or UDP <device_id> <data> and press Enter")
    print("Ctrl-C to exit\n")

    while True:
        try:
            cmd = input("AT> ").strip()
            if not cmd:
                continue

            elif cmd.startswith("UDP "):
                continue

            modem.command(cmd)

        except ATError as e:
            print(f"[ERROR] {e}")

        except KeyboardInterrupt:
            print("\nExiting...")
            break

# Main
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python3 at_udp_tool.py /dev/cu.usbmodemXXXXX")
        sys.exit(1)

    port = sys.argv[1]
    modem = ATModem(port)

    try:
        # Safe startup
        modem.command("AT")
        modem.command("ATE0")
        modem.command("AT+CMEE=2")

        # Enable UDP receive + logging
        modem.init_udp()

        time.sleep(1)
        modem.command("AT")
        modem.command("AT+CPIN?")
        modem.command("AT+CGDCONT?")
        modem.command('AT+CGDCONT=1,"IP","internet"')
        modem.command("AT+CEREG=2")
        modem.command("AT+CEREG?")
        modem.command("AT+CREG?")
        modem.command("AT+CGREG?")
        modem.command("AT#SGACT=1,1") # Use once, after power up or reset
        modem.command("AT+CGATT=1")
        modem.command("AT+CGATT?")
        time.sleep(5)
        modem.send_udp()
        time.sleep(1)

        repl(modem)

    finally:
        modem.close()
