#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <WebServer.h>
#include <HTTPClient.h>
#include "camera_pins.h"
#include "config.h"

// Server ports
#define VIDEO_PORT 80
#define SENSOR_PORT 81
#define CONTROL_PORT 82

// Create servers
WebServer videoServer(VIDEO_PORT);
WebServer sensorServer(SENSOR_PORT);
WebServer controlServer(CONTROL_PORT);

// Handle camera capture request
void handleCameraCapture() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        videoServer.send(500, "application/json", "{\"error\":\"Camera capture failed\"}");
        return;
    }
    
    videoServer.setContentLength(fb->len);
    videoServer.send(200, "image/jpeg", "");
    videoServer.sendContent((const char *)fb->buf, fb->len);
    
    esp_camera_fb_return(fb);
}

// Handle 404 errors
void handleNotFound() {
    String message = "{\"error\":\"Not found\"}";
    videoServer.send(404, "application/json", message);
}

// Handle robot control commands
void handleControl() {
    String cmd = controlServer.arg("command");
    if (cmd.length() > 0) {
        char sendCmd = ' ';
        if (cmd == "FORWARD") sendCmd = 'F';
        else if (cmd == "BACKWARD") sendCmd = 'B';
        else if (cmd == "LEFT") sendCmd = 'L';
        else if (cmd == "RIGHT") sendCmd = 'R';
        else if (cmd == "STOP") sendCmd = 'S';
        else if (cmd == "CENTER") sendCmd = 'C';
        else if (cmd == "AUTO") sendCmd = 'A';
        else if (cmd == "MANUAL") sendCmd = 'M';
        
        if (sendCmd != ' ') {
            sendToArduino(sendCmd);
            controlServer.send(200, "application/json", 
                "{\"status\":\"success\",\"command\":\"" + cmd + "\"}");
        } else {
            controlServer.send(400, "application/json", 
                "{\"error\":\"Invalid command\"}");
        }
    } else {
        controlServer.send(400, "application/json", 
            "{\"error\":\"No command provided\"}");
    }
}

// Store latest sensor data
String latestSensorData = "";

// Handle sensor data update
void handleSensorData(const String& data) {
    latestSensorData = data;  // Store raw sensor data
}

// Serve sensor data via HTTP endpoint
void handleSensorDataRequest() {
    String jsonData = "{\"status\":\"success\",\"data\":{";
    
    if (latestSensorData.length() > 0) {
        // Parse sensor data format: "D:distance,A:accX|accY|accZ,G:gyroX|gyroY|gyroZ"
        int dIndex = latestSensorData.indexOf("D:");
        int aIndex = latestSensorData.indexOf(",A:");
        int gIndex = latestSensorData.indexOf(",G:");
        
        String distance = "0", accX = "0", accY = "0", accZ = "0", gyroX = "0", gyroY = "0", gyroZ = "0";
        
        if (dIndex >= 0 && aIndex >= 0) {
            distance = latestSensorData.substring(dIndex + 2, aIndex);
        }
        
        if (aIndex >= 0 && gIndex >= 0) {
            String accData = latestSensorData.substring(aIndex + 3, gIndex);
            int pipe1 = accData.indexOf('|');
            int pipe2 = accData.lastIndexOf('|');
            if (pipe1 >= 0 && pipe2 >= 0 && pipe1 != pipe2) {
                accX = accData.substring(0, pipe1);
                accY = accData.substring(pipe1 + 1, pipe2);
                accZ = accData.substring(pipe2 + 1);
            }
        }
        
        if (gIndex >= 0) {
            String gyroData = latestSensorData.substring(gIndex + 3);
            int pipe1 = gyroData.indexOf('|');
            int pipe2 = gyroData.lastIndexOf('|');
            if (pipe1 >= 0 && pipe2 >= 0 && pipe1 != pipe2) {
                gyroX = gyroData.substring(0, pipe1);
                gyroY = gyroData.substring(pipe1 + 1, pipe2);
                gyroZ = gyroData.substring(pipe2 + 1);
            }
        }
        
        jsonData += "\"distance\":" + distance + ",";
        jsonData += "\"acceleration\":{\"x\":" + accX + ",\"y\":" + accY + ",\"z\":" + accZ + "},";
        jsonData += "\"gyroscope\":{\"x\":" + gyroX + ",\"y\":" + gyroY + ",\"z\":" + gyroZ + "}";
    }
    
    jsonData += "}}";
    sensorServer.send(200, "application/json", jsonData);
}

// Initialize all web servers
void setupServers() {
    // Video Server Setup
    videoServer.enableCORS(true);
    videoServer.on("/capture", HTTP_GET, handleCameraCapture);
    videoServer.onNotFound(handleNotFound);
    
    // Control Server Setup
    controlServer.enableCORS(true);
    controlServer.on("/control", HTTP_GET, handleControl);
    controlServer.onNotFound(handleNotFound);
    
    // Sensor Server Setup
    sensorServer.enableCORS(true);
    sensorServer.on("/data", HTTP_GET, handleSensorDataRequest);
    sensorServer.onNotFound(handleNotFound);
    
    // Start servers
    videoServer.begin();
    sensorServer.begin();
    controlServer.begin();
    
    Serial.println("[INFO] HTTP servers started");
    Serial.println("Video Server: " + WiFi.localIP().toString() + ":" + VIDEO_PORT);
    Serial.println("Sensor Server: " + WiFi.localIP().toString() + ":" + SENSOR_PORT);
    Serial.println("Control Server: " + WiFi.localIP().toString() + ":" + CONTROL_PORT);
}

#endif // SERVER_CONFIG_H
