#!/bin/bash

MEASUREMENT_DIR="/home/pi/Measurements"

mkdir "$MEASUREMENT_DIR"

cd "$MEASUREMENT_DIR"

mkdir UART
mkdir Wi-Fi
mkdir MQTT
mkdir Bluetooth
mkdir Wi-Fi_MQTT
mkdir Wi-Fi_Bluetooth
mkdir MQTT_Bluetooth
mkdir Wi-Fi_MQTT_Bluetooth
