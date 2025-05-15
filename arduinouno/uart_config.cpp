#include "uart_config.h"

// Create Software Serial instance
SoftwareSerial espSerial(UNO_RX_PIN, UNO_TX_PIN);

void setupUART() {
    // Initialize debug communication
    Serial.begin(DEBUG_BAUD_RATE);
    
    // Initialize ESP32 communication
    espSerial.begin(BAUD_RATE);
    Serial.println("[INFO] UART Software initialized");
    Serial.println("[INFO] RX pin: " + String(UNO_RX_PIN) + " <-- ESP32 TX (GPIO15)");
    Serial.println("[INFO] TX pin: " + String(UNO_TX_PIN) + " --> ESP32 RX (GPIO14)");
}

void sendToESP(const String& data) {
    espSerial.println(data);
    espSerial.flush();  // Wait until all data is sent
    Serial.println("[DEBUG] Sent data: " + data);
}

bool hasCommand() {
    return espSerial.available() > 0;
}

char readCommand() {
    if (hasCommand()) {
        char cmd = espSerial.read();
        Serial.println("[DEBUG] Received command: " + String(cmd));
        return cmd;
    }
    return '\0';
}
