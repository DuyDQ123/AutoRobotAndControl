#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include <Arduino.h>
#include <SoftwareSerial.h>

// UART Software pins for Arduino UNO
#define UNO_RX_PIN 6  // Connect to ESP32 TX (GPIO15)
#define UNO_TX_PIN 5  // Connect to ESP32 RX (GPIO14)
#define BAUD_RATE 9600
#define DEBUG_BAUD_RATE 115200

// Create Software Serial instance
extern SoftwareSerial espSerial;

// Initialize UART communication
void setupUART();

// Send data to ESP32
void sendToESP(const String& data);

// Check if data available from ESP32
bool hasCommand();

// Read command from ESP32
char readCommand();

#endif // UART_CONFIG_H
