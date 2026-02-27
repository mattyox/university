#!/usr/bin/env python3

import serial
import threading
import queue
import time
import sys
import re
from datetime import datetime

# Config
SMS_LOG_FILE = "sms.log"

# Exceptions
class ATError(Exception):
    pass

# Logging
def log_sms(direction, peer, text):
    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    line = f"[{ts}] {direction:<8} {peer} | {text}\n"
    with open(SMS_LOG_FILE, "a", encoding="utf-8") as f:
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
        self.sms_queue = queue.Queue()  # Incoming SMS indexes
        self.alive = True

        self.cmd_lock = threading.Lock()

        # Threads
        self.rx_thread = threading.Thread(target=self._reader, daemon=True)
        self.rx_thread.start()

        self.sms_worker = threading.Thread(target=self._sms_worker, daemon=True)
        self.sms_worker.start()

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
                self.sms_queue.put(index)
            except Exception as e:
                print(f"[SMS ERROR] {e}")

    # SMS handling
    def init_sms(self):
        self.command("AT+CMGF=1")             # Text mode
        self.command('AT+CSCS="GSM"')         # GSM charset
        self.command("AT+CSMP=17,167,0,0")
        self.command("AT+CNMI=2,1,0,0,0")     # New SMS URCs

    def send_sms(self, number, text, timeout=30):
        with self.cmd_lock:
            self._flush_rx()
            self.ser.write(f'AT+CMGS="{number}",145\r'.encode())

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
                raise ATError("No SMS prompt")

            time.sleep(0.05)    # Slight delay before sending text

            # Send text + Ctrl-Z
            self.ser.write(text.encode())
            self.ser.write(b'\x1A') # Sent
            # self.ser.write(b'\x1B') # Exit

            # Wait for +CMGS and OK
            t0 = time.time()
            while time.time() - t0 < timeout:
                try:
                    line = self.rx_queue.get(timeout=0.5)
                except queue.Empty:
                    continue
                if line.startswith("+CMGS"):
                    continue
                if line == "OK":
                    print("SMS sent")
                    log_sms("SENT", f"To:{number}", text)
                    return
                if "ERROR" in line:
                    raise ATError(line)

            raise ATError("Timeout waiting for SMS send completion")

    def read_sms(self, index):
        lines = self.command(f"AT+CMGR={index}")

        header = lines[0]
        text = lines[1] if len(lines) > 1 else ""

        status_match = re.search(r'"(REC [A-Z]+)"', header)
        number_match = re.search(r'"(\+?[0-9]+)"', header)

        status = status_match.group(1) if status_match else "UNKNOWN"
        sender = number_match.group(1) if number_match else "UNKNOWN"

        print("\n--- SMS RECEIVED ---")
        print(f"From: {sender}")
        print(text)
        print("--------------------\n")

        return status, sender, text

    def delete_sms(self, index):
        self.command(f"AT+CMGD={index}")

    # SMS worker (reads and logs)
    def _sms_worker(self):
        while self.alive:
            try:
                index = self.sms_queue.get(timeout=1)

            except queue.Empty:
                continue

            try:
                _, sender, text = self.read_sms(index)
                log_sms("RECEIVED", f"From:{sender}", text)

                # Delete SMS first
                self.delete_sms(index)

            except Exception as e:
                print(f"[SMS ERROR] {e}")

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
        self.sms_worker.join(timeout=1)
        self.ser.close()

# Interactive REPL
def repl(modem):
    print("\nAT and SMS Test Tool")
    print("Type AT commands or SMS <number, starting from +> <text> and press Enter")
    print("Ctrl-C to exit\n")

    while True:
        try:
            cmd = input("AT> ").strip()
            if not cmd:
                continue

            elif cmd.startswith("SMS "):
                _, number, text = cmd.split(" ", 2)
                modem.send_sms(number, text)

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
        print("  python3 at_sms_tool.py <port>")
        sys.exit(1)

    port = sys.argv[1]
    modem = ATModem(port)

    try:
        # Safe startup
        modem.command("AT")
        modem.command("ATE0")
        modem.command("AT+CMEE=2")

        # Enable SMS receive + logging
        modem.init_sms()

        repl(modem)

    finally:
        modem.close()
