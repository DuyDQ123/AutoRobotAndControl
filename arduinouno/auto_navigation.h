#ifndef AUTO_NAVIGATION_H
#define AUTO_NAVIGATION_H

#include "servo_control.h"
#include "sensors.h"

// Navigation parameters
#define MIN_DISTANCE 15       // Minimum safe distance in cm
#define TILT_THRESHOLD 8000   // Accelerometer threshold for detecting tilt/collision
#define ROTATION_THRESHOLD 50 // Gyroscope threshold for detecting rotation

// External sensor variables from sensors.cpp
extern int16_t ax, ay, az;
extern int16_t gx, gy, gz;

// Function declarations
void autoNavigate();

#endif // AUTO_NAVIGATION_H
