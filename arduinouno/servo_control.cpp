#include "servo_control.h"

// Create servo objects
Servo servoLeft, servoRight;

void setupServos() {
    servoLeft.attach(LEFT_SERVO_PIN);
    servoRight.attach(RIGHT_SERVO_PIN);
    stopMotors(); // Start with motors stopped
}

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
