#include "sensors.h"

// Create sensor objects
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
MPU6050 mpu;

// Global variables
int16_t ax, ay, az;
int16_t gx, gy, gz;
int lastDistance = 0;

// Calibration offsets
int16_t ax_offset, ay_offset, az_offset;
int16_t gx_offset, gy_offset, gz_offset;

void calibrateMPU() {
    Serial.println("Calibrating MPU6050...");
    
    // Take multiple readings to get average offset
    long ax_total = 0, ay_total = 0, az_total = 0;
    long gx_total = 0, gy_total = 0, gz_total = 0;
    int readings = 100;
    
    for (int i = 0; i < readings; i++) {
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        ax_total += ax;
        ay_total += ay;
        az_total += az;
        gx_total += gx;
        gy_total += gy;
        gz_total += gz;
        delay(10);
    }
    
    // Calculate offsets
    ax_offset = ax_total / readings;
    ay_offset = ay_total / readings;
    az_offset = (az_total / readings) - 16384; // Remove gravity
    gx_offset = gx_total / readings;
    gy_offset = gy_total / readings;
    gz_offset = gz_total / readings;
    
    Serial.println("Calibration complete!");
    Serial.println("Offsets: ");
    Serial.print("ax: "); Serial.println(ax_offset);
    Serial.print("ay: "); Serial.println(ay_offset);
    Serial.print("az: "); Serial.println(az_offset);
    Serial.print("gx: "); Serial.println(gx_offset);
    Serial.print("gy: "); Serial.println(gy_offset);
    Serial.print("gz: "); Serial.println(gz_offset);
}

void setupSensors() {
    // Initialize I2C for MPU6050
    Wire.begin();
    mpu.initialize();
    
    // Check MPU6050 connection
    if (!mpu.testConnection()) {
        Serial.println("MPU6050 NOT CONNECTED");
        return;
    }
    
    Serial.println("MPU6050 READY");
    
    // Calibrate on startup
    calibrateMPU();
}

int getDistance() {
    int dist = sonar.ping_cm();
    if (dist == 0) dist = lastDistance; // If error, use last value
    lastDistance = dist;
    return dist;
}

void updateMPU() {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Remove offsets from readings
    ax -= ax_offset;
    ay -= ay_offset;
    az -= az_offset;
    gx -= gx_offset;
    gy -= gy_offset;
    gz -= gz_offset;
}

String getSensorData() {
    int dist = getDistance();
    updateMPU();
    
    String data = "D:";
    data += String(dist);
    data += ",A:";
    data += String(ax) + "|" + String(ay) + "|" + String(az);
    data += ",G:";
    data += String(gx) + "|" + String(gy) + "|" + String(gz);
    
    return data;
}
