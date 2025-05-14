#include <WiFi.h>
#include <esp_camera.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include "camera_pins.h"
#include "config.h"

// Global variables
bool robotMode = false;  // false = manual, true = auto

// Create two servers on different ports
WebServer videoServer(80);  // Video streaming server
WebServer sensorServer(81); // Sensor data and control server

void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // Initialize with optimized settings for streaming
    if (psramFound()) {
        config.frame_size = FRAMESIZE_VGA;  // Lower resolution for better streaming
        config.jpeg_quality = 15;           // Slightly lower quality for faster transmission
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 20;           // Lower quality if no PSRAM
        config.fb_count = 1;
    }

    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    // Set initial camera settings
    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_VGA);
    s->set_quality(s, 15);
    s->set_brightness(s, 1);      // Increase brightness slightly
    s->set_saturation(s, 0);      // Normal saturation
    s->set_contrast(s, 0);        // Normal contrast
}

void handleCommand(String cmd) {
    Serial.println("\n[WEB] Command received: " + cmd);
    Serial.println("[DEBUG] Current mode before command: " + String(robotMode ? "AUTO" : "MANUAL"));
}

void setupServers() {
    // Video Server Setup
    videoServer.enableCORS(true);
    
    videoServer.on("/capture", HTTP_GET, []() {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            videoServer.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Camera capture failed\"}");
            return;
        }
        
        videoServer.setContentLength(fb->len);
        videoServer.send(200, "image/jpeg", "");
        videoServer.sendContent((const char *)fb->buf, fb->len);
        
        esp_camera_fb_return(fb);
    });

    videoServer.onNotFound([]() {
        videoServer.send(404, "application/json", "{\"status\":\"error\",\"message\":\"Not found\"}");
    });

    // Sensor & Control Server Setup
    sensorServer.enableCORS(true);
    
    sensorServer.on("/control", HTTP_GET, []() {
        Serial.println("\n[WEB] HTTP request on /control");
        String cmd = sensorServer.arg("command");
        Serial.println("[DEBUG] Command parameter: " + cmd);
        
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
                // Debug before sending
                Serial.println("[DEBUG] Sending command: " + String(sendCmd));
                
                Serial.write(sendCmd); // Send command to Arduino
                Serial.flush();        // Wait until data is sent
                
                // Update mode tracking if AUTO/MANUAL command
                if (sendCmd == 'A') robotMode = true;
                else if (sendCmd == 'M') robotMode = false;
                
                // Debug after sending
                Serial.println("[DEBUG] Command sent successfully");
                Serial.println("[DEBUG] New mode: " + String(robotMode ? "AUTO" : "MANUAL"));
                handleCommand(cmd);
                sensorServer.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Command sent: " + cmd + "\"}");
            } else {
                sensorServer.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid command\"}");
            }
        } else {
            sensorServer.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No command provided\"}");
        }
    });

    sensorServer.onNotFound([]() {
        sensorServer.send(404, "application/json", "{\"status\":\"error\",\"message\":\"Not found\"}");
    });
    
    // Start both servers
    videoServer.begin();
    sensorServer.begin();
    Serial.println("HTTP servers started");
    Serial.println("Video Server IP: " + WiFi.localIP().toString() + ":80");
    Serial.println("Sensor Server IP: " + WiFi.localIP().toString() + ":81");
}

void setup() {
    // Configure hardware Serial for communication with Arduino
    // Use GPIO 1 (TX) and GPIO 3 (RX) for Serial
    Serial.begin(9600, SERIAL_8N1, 3, 1); // RX=GPIO3, TX=GPIO1
    Serial.setTimeout(1);  // Faster timeout for Serial reads
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.println(WiFi.localIP());

    initCamera();
    setupServers();
}

void parseSensorData(String data) {
    // Format: "D:distance,A:accX|accY|accZ,G:gyroX|gyroY|gyroZ"
    String distance, accX, accY, accZ, gyroX, gyroY, gyroZ;
    
    // Parse distance
    int dIndex = data.indexOf("D:");
    int aIndex = data.indexOf(",A:");
    if (dIndex >= 0 && aIndex >= 0) {
        distance = data.substring(dIndex + 2, aIndex);
    }
    
    // Parse accelerometer data
    int gIndex = data.indexOf(",G:");
    if (aIndex >= 0 && gIndex >= 0) {
        String accData = data.substring(aIndex + 3, gIndex);
        int pipe1 = accData.indexOf('|');
        int pipe2 = accData.lastIndexOf('|');
        if (pipe1 >= 0 && pipe2 >= 0 && pipe1 != pipe2) {
            accX = accData.substring(0, pipe1);
            accY = accData.substring(pipe1 + 1, pipe2);
            accZ = accData.substring(pipe2 + 1);
        }
    }
    
    // Parse gyroscope data
    if (gIndex >= 0) {
        String gyroData = data.substring(gIndex + 3);
        int pipe1 = gyroData.indexOf('|');
        int pipe2 = gyroData.lastIndexOf('|');
        if (pipe1 >= 0 && pipe2 >= 0 && pipe1 != pipe2) {
            gyroX = gyroData.substring(0, pipe1);
            gyroY = gyroData.substring(pipe1 + 1, pipe2);
            gyroZ = gyroData.substring(pipe2 + 1);
        }
    }
    
    // Send to web server
    if (distance.length() > 0) {
        HTTPClient http;
        String url = String(WEBSERVER_URL) + ":81/save_sensor_data.php";
        url += "?distance=" + distance;
        url += "&ax=" + accX + "&ay=" + accY + "&az=" + accZ;
        url += "&gx=" + gyroX + "&gy=" + gyroY + "&gz=" + gyroZ;
        
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
        }
        
        http.end();
    }
}

void loop() {
    // Read commands from Serial Monitor
    if (Serial.available()) {
        String data = Serial.readStringUntil('\n');
        if (data.length() > 0) {
            // Debug print
            Serial.println("\n[DEBUG] Received data: " + data);
            parseSensorData(data);
        }
    }

    // Handle any WiFi disconnections
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
        delay(500);
    }
    
    // Handle requests for both servers
    videoServer.handleClient();
    sensorServer.handleClient();
}