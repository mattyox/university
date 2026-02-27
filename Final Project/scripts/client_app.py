from zeroconf import ServiceBrowser, ServiceListener, Zeroconf
import time
import socket
import struct
import time


service = "_energy_meter._udp.local."
host = None
port = None
device_found = False
udp_socket = None
buffer_size = 1024


class Listener(ServiceListener):

    def update_service(self, zc: Zeroconf, type_: str, name: str) -> None:
        print(f"Service {name} updated.")

    def remove_service(self, zc: Zeroconf, type_: str, name: str) -> None:
        print(f"Service {name} removed.")

    def add_service(self, zc: Zeroconf, type_: str, name: str) -> None:
        print(f"Service {name} added.")

        info = zc.get_service_info(type_, name)
        
        global host 
        host = info.server[:len(info.server)-1]
        print(f"Host: {host}")

        global port
        port = info.port
        print(f"Port: {port}")

        global device_found
        device_found = True


def udp_transceive_double(command: bytes):
    try:
        udp_socket.sendto(command, (host, port))
    except:
        print(f"Could not send message to the host: {host} and port: {port}")

    try:
        server_response = udp_socket.recvfrom(buffer_size)[0]
    except:
        print(f"Could not recive message from the host: {host} and port: {port}")

    if server_response[0:1] == command:
        value = struct.unpack('d', server_response[1:])[0]
        return value
    else:
        print("Error. Invalid response!")
        raise ValueError


def get_active_energy():
    print("Getting active energy...")

    try:
        active_energy = udp_transceive_double(b'\x00')
    except ValueError:
        print("Error. Could not get active energy value!")
    else:
        print("Got active energy value: {0:.3f} [kWh]".format(active_energy))


def get_reactive_energy():
    print("Getting reactive energy...")

    try:
        reactive_energy = udp_transceive_double(b'\x01')
    except ValueError:
        print("Error. Could not get reactive energy value!")
    else:
        print("Got reactive energy value: {0:.3f} [kVARh]".format(reactive_energy))


def get_apparent_energy():
    print("Getting apparent energy...")

    try:
        apparent_energy = udp_transceive_double(b'\x02')
    except ValueError:
        print("Error. Could not get apparent energy value!")
    else:
        print("Got apparent energy value: {0:.3f} [kVAh]".format(apparent_energy))


def get_phase_1_current():
    print("Getting phase 1 rms current...")

    try:
        current = udp_transceive_double(b'\x03')
    except ValueError:
        print("Error. Could not get phase 1 rms current value!")
    else:
        print("Got phase 1 rms current value: {0:.3f} [A]".format(current))


def get_phase_1_voltage():
    print("Getting phase 1 rms voltage...")

    try:
        voltage = udp_transceive_double(b'\x04')
    except ValueError:
        print("Error. Could not get phase 1 rms voltage value!")
    else:
        print("Got phase 1 rms current value: {0:.3f} [V]".format(voltage))


def get_phase_2_current():
    print("Getting phase 2 rms current...")

    try:
        current = udp_transceive_double(b'\x05')
    except ValueError:
        print("Error. Could not get phase 2 rms current value!")
    else:
        print("Got phase 2 rms current value: {0:.3f} [A]".format(current))


def get_phase_2_voltage():
    print("Getting phase 2 rms voltage...")

    try:
        voltage = udp_transceive_double(b'\x06')
    except ValueError:
        print("Error. Could not get phase 2 rms voltage value!")
    else:
        print("Got phase 2 rms current value: {0:.3f} [V]".format(voltage))


def get_phase_3_current():
    print("Getting phase 3 rms current...")

    try:
        current = udp_transceive_double(b'\x07')
    except ValueError:
        print("Error. Could not get phase 3 rms current value!")
    else:
        print("Got phase 3 rms current value: {0:.3f} [A]".format(current))


def get_phase_3_voltage():
    print("Getting phase 3 rms voltage...")

    try:
        voltage = udp_transceive_double(b'\x08')
    except ValueError:
        print("Error. Could not get phase 3 rms voltage value!")
    else:
        print("Got phase 3 rms current value: {0:.3f} [V]".format(voltage))


def udp_transceive_time(command: bytes):
    try:
        udp_socket.sendto(command, (host, port))
    except:
        print(f"Could not send message to the host: {host} and port: {port}")

    try:
        server_response = udp_socket.recvfrom(buffer_size)[0]
    except:
        print(f"Could not recive message from the host: {host} and port: {port}")

    if server_response[0:1] == command:
        t = time.ctime(struct.unpack('q', server_response[1:])[0])
        return t
    else:
        print("Error. Invalid response!")
        raise ValueError


def get_boot_time():
    print("Getting last boot time...")

    try:
        time = udp_transceive_time(b'\x09')
    except ValueError:
        print("Error. Could not get last reset time!")
    else:
        print(f"Got last boot time: {time}")


def get_reset_time():
    print("Getting last reset time...")

    try:
        time = udp_transceive_time(b'\x0A')
    except ValueError:
        print("Error. Could not get last reset time!")
    else:
        print(f"Got last reset time: {time}")


def reset_readings():
    print("Reseting readings...")

    command = b'\x0B'

    try:
        udp_socket.sendto(command, (host, port))
    except:
        print(f"Could not send message to the host: {host} and port: {port}")

    try:
        server_response = udp_socket.recvfrom(buffer_size)[0]
    except:
        print(f"Could not recive message from the host: {host} and port: {port}")

    if server_response[0:1] == command:
        print("Reset module readings.")
    else:
        print("Error. Invalid response!")


def command_switch(a_command):
    if a_command == "1":
        get_active_energy()
    elif a_command == "2":
        get_reactive_energy()
    elif a_command == "3":
        get_apparent_energy()
    elif a_command == "4":
        get_phase_1_current()
    elif a_command == "5":
        get_phase_1_voltage()
    elif a_command == "6":
        get_phase_2_current()
    elif a_command == "7":
        get_phase_2_voltage()
    elif a_command == "8":
        get_phase_3_current()
    elif a_command == "9":
        get_phase_3_voltage()
    elif a_command == "10":
        get_boot_time()
    elif a_command == "11":
        get_reset_time()
    elif a_command == "12":
        reset_readings()
    elif a_command == "0":
        exit()
    else:
        print("Warning. Unknown command!")


def main():
    print("Welcome to the energy meter app!\n")

    zeroconf = Zeroconf()
    listener = Listener()
    browser = ServiceBrowser(zeroconf, service, listener)

    while device_found == False:
        print("Waiting for service dicover...")
        time.sleep(1.0)

    browser.cancel()

    global udp_socket
    udp_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)

    while True:
        print("\nWhat do you want to do?\n"
        "1. Get active energy\n"
        "2. Get reactive energy\n"
        "3. Get apparent energy\n"
        "4. Get phase 1 rms current\n"
        "5. Get phase 1 rms voltage\n"
        "6. Get phase 2 rms current\n"
        "7. Get phase 2 rms voltage\n"
        "8. Get phase 3 rms current\n"
        "9. Get phase 3 rms voltage\n"
        "10. Get last boot time\n"
        "11. Get last reset time\n"
        "12. Reset readings\n"
        "0. Exit\n")

        command = input("Selected command: ")

        command_switch(command)


main()
