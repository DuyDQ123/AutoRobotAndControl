#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Servo.h>

// Servo pins
#define LEFT_SERVO_PIN 11
#define RIGHT_SERVO_PIN 10

// Function declarations
extern Servo servoLeft, servoRight;
void setupServos();
void forward();
void backward();
void turnLeft();
void turnRight();
void stopMotors();

#endif // SERVO_CONTROL_H
