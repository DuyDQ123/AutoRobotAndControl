#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include <HardwareSerial.h>

// UART pins for ESP32CAM
#define ESP_RX_PIN 14  // Connect to Arduino TX5
#define ESP_TX_PIN 15  // Connect to Arduino RX6
#define UART_NUM 2     // Using UART2
#define BAUD_RATE 9600

// Create Hardware Serial instance
HardwareSerial arduinoSerial(UART_NUM);

// Initialize UART communication
void setupUART() {
    arduinoSerial.begin(BAUD_RATE, SERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN);
    Serial.println("[INFO] UART initialized");
}

// Send data to Arduino
void sendToArduino(const char command) {
    arduinoSerial.write(command);
    Serial.printf("[DEBUG] Sent command to Arduino: %c\n", command);
}

// Read data from Arduino
String readFromArduino() {
    String data = "";
    if (arduinoSerial.available()) {
        data = arduinoSerial.readStringUntil('\n');
        Serial.printf("[DEBUG] Received from Arduino: %s\n", data.c_str());
    }
    return data;
}

#endif // UART_CONFIG_H
