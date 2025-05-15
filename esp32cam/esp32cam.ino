#include <WiFi.h>
#include "config.h"
#include "camera_config.h"
#include "uart_config.h"
#include "server_config.h"

// Global variables
bool robotMode = false;  // false = manual, true = auto

void setup() {
    // Initialize debug serial port
    Serial.begin(115200);
    Serial.println("\n[INFO] Starting ESP32-CAM...");

    // Initialize UART communication with Arduino
    setupUART();
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[INFO] Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[INFO] WiFi Connected!");
    Serial.println("IP Address: " + WiFi.localIP().toString());

    // Initialize Camera
    if (!setupCamera()) {
        Serial.println("[ERROR] Camera setup failed");
        return;
    }

    // Initialize Web Servers
    setupServers();
    
    Serial.println("[INFO] Setup complete!");
}

void loop() {
    // Handle incoming data from Arduino
    String data = readFromArduino();
    if (data.length() > 0) {
        handleSensorData(data);
    }

    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WARN] WiFi disconnected, reconnecting...");
        WiFi.reconnect();
        delay(500);
    }
    
    // Handle web server clients
    videoServer.handleClient();
    sensorServer.handleClient();
    controlServer.handleClient();
    
    // Small delay to prevent watchdog timeouts
    delay(1);
}
