#include "uart_config.h"
#include "servo_control.h"
#include "sensors.h"
#include "auto_navigation.h"

// Operation mode
bool autoMode = false;     // false = manual, true = auto

// Timing variables
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 500; // Update interval in ms

void setup() {
    // Initialize both Serial communications
    setupUART();
    
    // Initialize hardware
    setupServos();
    setupSensors();
    
    Serial.println("[INFO] All systems initialized");
}

void loop() {
    // Check for commands from ESP32-CAM
    if (hasCommand()) {
        handleCommand(readCommand());
    }

    // Run auto navigation if in auto mode
    if (autoMode) {
        autoNavigate();
    }

    // Update and send sensor data periodically
    if (millis() - lastUpdate >= UPDATE_INTERVAL) {
        String sensorData = getSensorData();
        sendToESP(sensorData);
        lastUpdate = millis();
    }
}

// Handle incoming commands
void handleCommand(char cmd) {
    if (cmd == '\0') return;  // Invalid command
    
    switch (cmd) {
        case 'F': 
            forward(); 
            Serial.println("[DEBUG] Moving forward");
            break;
        case 'B': 
            backward();
            Serial.println("[DEBUG] Moving backward");
            break;
        case 'L': 
            turnLeft();
            Serial.println("[DEBUG] Turning left");
            break;
        case 'R': 
            turnRight();
            Serial.println("[DEBUG] Turning right");
            break;
        case 'S': 
            stopMotors();
            Serial.println("[DEBUG] Stopping");
            break;
        case 'A':
            autoMode = true;
            Serial.println("[DEBUG] Switching to AUTO mode");
            break;
        case 'M': 
            autoMode = false;
            stopMotors(); // Stop when switching to manual
            Serial.println("[DEBUG] Switching to MANUAL mode");
            break;
        default: 
            Serial.println("[ERROR] Unknown command: " + String(cmd));
            break;
    }
}
