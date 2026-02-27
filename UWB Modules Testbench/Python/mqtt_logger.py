import paho.mqtt.client as mqtt
from datetime import datetime, timedelta
import argparse

def log_message(message, log_file):
    now = datetime.now()
    timestamp = now.strftime("[%H:%M:%S.") + f"{now.microsecond:06d}]"
    output_line = f"{timestamp} {message}"

    log_file.write(output_line + "\n")
    log_file.flush()

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker.")

        for topic in userdata['topics']:
            client.subscribe(topic)
            print(f"Logging MQTT messages at {topic}")
    else:
        print(f"Failed to connect, return code {rc}")

def on_message(client, userdata, message):
    msg = message.payload.decode(errors='ignore')
    log_message(f"{msg}", userdata['log_file'])

def main(broker, port, topics, log_file):
    with open(log_file, "w") as lf:
        client = mqtt.Client()

        # Set user data for callbacks, including the open log file handle
        client.user_data_set({'topics': topics, 'log_file': lf})

        client.on_connect = on_connect
        client.on_message = on_message

        try:
            client.connect(broker, port, 60)

            start_time = datetime.now()
            end_time = start_time + timedelta(minutes=10)
            
            while datetime.now() < end_time:
                client.loop(timeout=0.0)

        except Exception as e:
            print(f"MQTT connection error: {e}")

        except KeyboardInterrupt:
            print("Exiting...")

        finally:
            print("Finished logging.")
            client.disconnect()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="MQTT logger with timestamps")
    parser.add_argument("-b", "--broker", default="localhost", help="MQTT broker address (default: %(default)s)")
    parser.add_argument("-p", "--port", type=int, default=1883, help="MQTT broker port (default: %(default)s)")
    parser.add_argument("-t", "--topics", nargs='+', default=["test/topic"], help="MQTT topics to subscribe to")
    parser.add_argument("-f", "--file", default="log_file.txt", help="Output log file path (default: %(default)s)")

    args = parser.parse_args()
    main(args.broker, args.port, args.topics, args.file)
