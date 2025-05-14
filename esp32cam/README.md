# ESP32-CAM Project

This directory contains the code for the ESP32-CAM module that handles video streaming and communication with the Arduino Uno.

## Hardware Requirements

- ESP32-CAM module
- FTDI programmer or USB-to-TTL converter for programming
- Jumper wires

## Pin Connections

### Serial Communication with Arduino
- ESP32 RX (GPIO3) ← Arduino TX (Pin 1)
- ESP32 TX (GPIO1) → Arduino RX (Pin 0)
- ESP32 GND → Arduino GND

Note: Using hardware Serial on GPIO1/GPIO3 (U0T/U0R pins) instead of GPIO16/17

### Programming Connections (with FTDI)
- ESP32 U0R → FTDI TX
- ESP32 U0T → FTDI RX
- ESP32 GND → FTDI GND
- ESP32 5V → FTDI VCC
- ESP32 GPIO0 → GND (for programming mode)

## Required Libraries

- WiFi.h (ESP32 core)
- esp_camera.h (ESP32 camera driver)
- HTTPClient.h (ESP32 core)
- WebServer.h (ESP32 core)

## Configuration

The following settings can be configured in `config.h`:
- WiFi credentials (SSID and password)
- Web server URL

## Features

1. Video Streaming Server (Port 80)
   - /capture endpoint for JPEG image capture

2. Sensor & Control Server (Port 81)
   - /control endpoint for robot movement commands
   - Forwards commands to Arduino via Serial2
   - Receives sensor data from Arduino
   - Forwards sensor data to web interface

## Debugging

The ESP32-CAM uses hardware Serial (UART0) for Arduino communication:
- Pins: GPIO1 (TX) and GPIO3 (RX)
- Baudrate: 9600