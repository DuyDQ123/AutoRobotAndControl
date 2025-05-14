# Arduino Uno Project

This repository contains an Arduino sketch for a project using the Arduino Uno board.

## Hardware Requirements

- Arduino Uno board
- ESP32-CAM module
- Servo motors (2x)
- Ultrasonic sensor (HC-SR04)
- MPU6050 sensor
- USB cable for programming
- Jumper wires

## Pin Connections

### Servo Motors
- Left Servo: Pin 11
- Right Servo: Pin 10

### Ultrasonic Sensor (HC-SR04)
- Trigger Pin: 8
- Echo Pin: 9

### MPU6050
- SDA: A4 (Arduino's I2C)
- SCL: A5 (Arduino's I2C)

### Serial Communication with ESP32-CAM
- Arduino TX (Pin 1) → ESP32 RX (GPIO16)
- Arduino RX (Pin 0) → ESP32 TX (GPIO17)
- Arduino GND → ESP32 GND

## Required Libraries

- Servo.h (Arduino built-in)
- NewPing.h (HC-SR04 ultrasonic sensor)
- Wire.h (Arduino built-in for I2C)
- MPU6050.h (MPU6050 accelerometer/gyroscope)

## Setup Instructions

1. Clone this repository or download the files
2. Open `arduinouno.ino` in the Arduino IDE
3. Connect your Arduino Uno board to your computer via USB
4. Select "Arduino Uno" from Tools > Board menu
5. Select the correct COM port from Tools > Port menu

## Usage

Upload the sketch to your Arduino Uno board using the upload button (→) in the Arduino IDE. The program will start running automatically after upload completes.

## Project Structure

- `arduinouno.ino`: Main Arduino sketch file containing the program code

## License

This project is open source and available under the MIT License.