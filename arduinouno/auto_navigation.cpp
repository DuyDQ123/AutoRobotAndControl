#include "auto_navigation.h"

// Smoothing for MPU readings
#define SMOOTH_FACTOR 0.1  // Lower = more smoothing
#define DEADZONE 20        // Ignore small gyro changes

// Navigation state
enum NavState {
    MOVING_FORWARD,
    AVOIDING_OBSTACLE,
    ADJUSTING_COURSE
};

NavState currentState = MOVING_FORWARD;
unsigned long lastStateChange = 0;
unsigned long lastCorrection = 0;
int turnDirection = 1;  // 1 for right, -1 for left

// Smoothed sensor values
float smoothedGz = 0;
int currentLeftSpeed = 90;   // Current servo positions
int currentRightSpeed = 90;

// Gradually change servo speed
void adjustServoSpeed(Servo& servo, int& current, int target, int step) {
    if (current < target) {
        current = min(current + step, target);
    } else if (current > target) {
        current = max(current - step, target);
    }
    servo.write(current);
}

void autoNavigate() {
    // Get sensor data
    int dist = getDistance();
    updateMPU();
    
    // Smooth gyro readings with debug output every 500ms
    smoothedGz = (smoothedGz * (1 - SMOOTH_FACTOR)) + (gz * SMOOTH_FACTOR);
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 500) {
        Serial.print("Raw gz: "); Serial.print(gz);
        Serial.print(" Smoothed: "); Serial.print(smoothedGz);
        Serial.print(" Left: "); Serial.print(currentLeftSpeed);
        Serial.print(" Right: "); Serial.println(currentRightSpeed);
        lastDebug = millis();
    }
    
    // Check for tilt/collision using accelerometer (with smoothing)
    static float smoothedAx = 0, smoothedAy = 0;
    smoothedAx = (smoothedAx * 0.8) + (ax * 0.2);
    smoothedAy = (smoothedAy * 0.8) + (ay * 0.2);
    bool isTilted = abs(smoothedAx) > TILT_THRESHOLD || abs(smoothedAy) > TILT_THRESHOLD;
    if (isTilted) {
        stopMotors();
        delay(1000);  // Wait a second if tilted
        backward();
        delay(500);
        return;
    }

    // State machine for navigation
    switch (currentState) {
        case MOVING_FORWARD:
            // Use smoothed gyro to keep straight
            if (abs(smoothedGz) > ROTATION_THRESHOLD && 
                millis() - lastCorrection > 100) {  // Limit correction frequency
                
                if (abs(smoothedGz) > DEADZONE) {  // Ignore small variations
                    if (smoothedGz > 0) {
                        // Gradual left correction
                        adjustServoSpeed(servoLeft, currentLeftSpeed, 170, 5);
                        adjustServoSpeed(servoRight, currentRightSpeed, 0, 5);
                    } else {
                        // Gradual right correction
                        adjustServoSpeed(servoLeft, currentLeftSpeed, 180, 5);
                        adjustServoSpeed(servoRight, currentRightSpeed, 10, 5);
                    }
                    lastCorrection = millis();
                }
            } else {
                // Gradually return to forward motion
                adjustServoSpeed(servoLeft, currentLeftSpeed, 180, 2);
                adjustServoSpeed(servoRight, currentRightSpeed, 0, 2);
            }
            
            // Check for obstacles
            if (dist < MIN_DISTANCE) {
                currentState = AVOIDING_OBSTACLE;
                lastStateChange = millis();
                stopMotors();
            }
            break;

        case AVOIDING_OBSTACLE:
            // Get rotation direction based on obstacles
            if (millis() - lastStateChange < 500) {
                stopMotors();  // Brief pause before turning
            }
            else if (millis() - lastStateChange < 1500) {
                // Turn in the chosen direction
                if (turnDirection > 0) {
                    turnRight();
                } else {
                    turnLeft();
                }
            }
            else {
                // After turning, switch direction for next time
                turnDirection = -turnDirection;
                currentState = MOVING_FORWARD;
                lastStateChange = millis();
            }
            break;

        case ADJUSTING_COURSE:
            // Use MPU data to realign
            if (abs(gz) < ROTATION_THRESHOLD) {
                currentState = MOVING_FORWARD;
                lastStateChange = millis();
            }
            else if (gz > 0) {
                turnLeft();  // Counter-rotate if spinning right
            }
            else {
                turnRight(); // Counter-rotate if spinning left
            }
            break;
    }

    // Check if we're rotating too much while moving
    if (currentState == MOVING_FORWARD && abs(gz) > ROTATION_THRESHOLD * 2) {
        currentState = ADJUSTING_COURSE;
        lastStateChange = millis();
    }
}
