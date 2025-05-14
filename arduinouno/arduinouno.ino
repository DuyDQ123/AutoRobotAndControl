#include <Servo.h>
#include <NewPing.h>
#include <Wire.h>
#include <MPU6050.h>

// Servo pins
Servo servoLeft, servoRight;
const int leftPin = 11;
const int rightPin = 10;

// Ultrasonic sensor (HC-SR04)
const int trigPin = 8;
const int echoPin = 9;
NewPing sonar(trigPin, echoPin, 200);

// MPU6050 sensor
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Last distance measurement
unsigned long lastUpdate = 0;
int lastDistance = 0;

// Mode: 0 = manual, 1 = auto
int mode = 0;

void setup() {
  // Initialize servo motors
  servoLeft.attach(leftPin);
  servoRight.attach(rightPin);
  
  // Start Serial communication with faster timeout
  Serial.begin(9600);
  Serial.setTimeout(1);

  // Initialize I2C for MPU6050
  Wire.begin();
  mpu.initialize();
  
  // Check MPU6050 connection
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 NOT CONNECTED");
  } else {
    Serial.println("MPU6050 READY");
  }
}

void loop() {
  // Check for incoming commands from ESP32-CAM
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'F') forward();
    else if (cmd == 'B') backward();
    else if (cmd == 'L') turnLeft();
    else if (cmd == 'R') turnRight();
    else if (cmd == 'S') stopMotors();
    else if (cmd == 'A') mode = 1;  // Set to auto mode
    else if (cmd == 'M') mode = 0;  // Set to manual mode
  }

  // If in Auto Mode, move based on sensor data
  if (mode == 1) {
    autoMode();
  }

  // Update sensors every 500ms
  if (millis() - lastUpdate >= 500) {
    // Measure distance from ultrasonic sensor
    int dist = sonar.ping_cm();
    if (dist == 0) dist = lastDistance; // If error in measurement, use last distance value
    lastDistance = dist;

    // Read values from MPU6050
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Send formatted data to ESP32-CAM
    // Format: "D:distance,A:accX|accY|accZ,G:gyroX|gyroY|gyroZ"
    // Format data into a single string before sending
    String data = "D:";
    data += String(dist);
    data += ",A:";
    data += String(ax) + "|" + String(ay) + "|" + String(az);
    data += ",G:";
    data += String(gx) + "|" + String(gy) + "|" + String(gz);
    
    // Send the complete line at once
    Serial.println(data);
    Serial.flush(); // Wait for data to be sent completely

    // Update time
    lastUpdate = millis();
  }
}

// Auto mode logic based on ultrasonic sensor distance
void autoMode() {
  int dist = sonar.ping_cm();

  if (dist == 0) dist = lastDistance;  // Use last distance if sensor error
  lastDistance = dist;

  if (dist < 15) {
    stopMotors(); // Stop if too close to an obstacle
    delay(500);   // Wait for a moment
    turnLeft();   // Turn left if too close
    delay(500);   // Wait after turning
  } 
  else if (dist >= 15 ) {
    forward();    // Move forward if there's enough space
  } 
  else {
    stopMotors(); // Stop if the distance is too far
  }
}

// Motor control functions
void forward() {
  servoLeft.write(180);  // Move left servo forward
  servoRight.write(0);   // Move right servo forward
}

void backward() {
  servoLeft.write(0);    // Move left servo backward
  servoRight.write(180); // Move right servo backward
}

void turnLeft() {
  servoLeft.write(90);   // Stop left servo
  servoRight.write(180); // Turn right servo
}

void turnRight() {
  servoLeft.write(180);  // Turn left servo
  servoRight.write(90);  // Stop right servo
}

void stopMotors() {
  servoLeft.write(90);   // Stop left servo
  servoRight.write(90);  // Stop right servo
}