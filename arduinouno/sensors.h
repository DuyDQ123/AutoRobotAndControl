#ifndef SENSORS_H
#define SENSORS_H

#include <NewPing.h>
#include <Wire.h>
#include <MPU6050.h>

// Ultrasonic sensor pins
#define TRIG_PIN 8
#define ECHO_PIN 9
#define MAX_DISTANCE 200

// Function declarations
extern NewPing sonar;
extern MPU6050 mpu;
extern int16_t ax, ay, az;
extern int16_t gx, gy, gz;
extern int lastDistance;

void setupSensors();
int getDistance();
void updateMPU();
String getSensorData();

#endif // SENSORS_H
