#!/bin/bash

SCRIPT_DIR="/home/pi/Python"
MEASUREMENT_DIR="/home/pi/Measurements"
PROTOCOL_DIR="/MQTT_Bluetooth"

MODULES_NUM="$1"

# Check if not set or less than 1
if [ -z "$MODULES_NUM" ] || [ "$MODULES_NUM" -lt 1 ]; then
    echo "Usage: $0 <modules number> (must be >= 1)"
    exit 1
fi

python3 "$SCRIPT_DIR/serial_logger.py" -f "$MEASUREMENT_DIR/$PROTOCOL_DIR/serial_log_${MODULES_NUM}.txt" &
python3 "$SCRIPT_DIR/mqtt_logger.py" -f "$MEASUREMENT_DIR/$PROTOCOL_DIR/mqtt_log_${MODULES_NUM}.txt" &
python3 "$SCRIPT_DIR/serial_logger.py" -p /dev/rfcomm0 -f "$MEASUREMENT_DIR/$PROTOCOL_DIR/bluetooth_log_${MODULES_NUM}.txt" &

wait
