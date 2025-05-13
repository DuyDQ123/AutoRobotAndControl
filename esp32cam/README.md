# ESP32-CAM Project

This project utilizes the ESP32-CAM module, a compact development board featuring an ESP32-S chip with an integrated camera and microSD card slot.

## Hardware Requirements

- ESP32-CAM module
- FTDI Programmer/USB-TTL converter for uploading code
- Micro USB cable for power
- Micro SD card (optional, for storing images)
- Breadboard and jumper wires

## Pin Connections

### Programming Mode
```
ESP32-CAM      FTDI Programmer
GND ----------- GND
5V  ----------- VCC (5V)
U0R ----------- TX
U0T ----------- RX
GPIO0 -------- GND (only during programming)
```

### Normal Operation Mode
- Remove GPIO0 to GND connection after programming
- Power can be supplied through 5V and GND pins

## Features

- Integrated OV2640 camera
- MicroSD card slot for image storage
- Built-in flash LED
- WiFi connectivity
- Programmable through Arduino IDE

## Setup Instructions

1. Install Required Libraries:
   - ESP32 board package in Arduino IDE
   - "ESP32 Camera" library

2. Arduino IDE Settings:
   - Board: "AI Thinker ESP32-CAM"
   - Upload Speed: 115200
   - Flash Frequency: 80MHz
   - Flash Mode: QIO
   - Partition Scheme: "Huge APP"

3. Programming Steps:
   - Connect GPIO0 to GND for programming mode
   - Press reset button on ESP32-CAM
   - Upload code
   - Disconnect GPIO0 from GND
   - Press reset to run program

## Common Applications

1. Video Streaming
2. Photo Capture
3. Motion Detection
4. Security Camera
5. Remote Monitoring

## Notes

- The ESP32-CAM needs a good 5V power supply for stable operation
- Built-in LED can be used as a flash or status indicator
- Default WiFi credentials can be modified in the code
- Camera resolution and quality can be adjusted based on requirements

## Troubleshooting

1. If upload fails:
   - Check GPIO0 connection to GND
   - Press reset button before uploading
   - Verify FTDI connections

2. If camera won't initialize:
   - Check power supply stability
   - Verify camera module connection
   - Try reformatting SD card if used

## Resources

- [ESP32-CAM Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-cam_datasheet_en.pdf)
- [Camera Web Server Example](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer)
- [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)