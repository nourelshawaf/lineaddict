/*
  ============================================================================
  Project: 5-Channel Line Follower Robot
  Board:   Arduino Nano / Arduino Uno
  Driver:  L298N Motor Driver
  Motors:  2x N20 DC Gear Motors
  Sensors: 5-Channel IR Sensor Array

  Description:
  This program controls a line follower robot using five IR sensors connected
  to analog pins A1-A5. The robot calculates the line position using weighted
  sensor values and applies PID correction to control the left and right motors.

  Important Hardware Note:
  - The motor polarity is reversed in this robot.
  - Negative speed values move the robot forward.
  - Motors are powered through the L298N motor driver.
  - Arduino and sensors must be powered from a stable 5V supply.
  - All grounds must be connected together.

  Author: Noureldeen Elshawaf
  License: MIT
  ============================================================================
*/


// ============================================================================
// Pin Configuration
// ============================================================================

// ---------- IR Sensor Pins ----------
// Sensor layout from left to right:
//
//   S1      S2      S3      S4      S5
//  Left            Center           Right
//
// Arduino pins:
//   A1      A2      A3      A4      A5
//
// Sensor weights:
//   -2      -1       0      +1      +2

#define S1 A1   // Left-most sensor
#define S2 A2
#define S3 A3   // Middle sensor
#define S4 A4
#define S5 A5   // Right-most sensor


// ---------- L298N Motor Driver Pins ----------

// Right motor
#define RMF 7   // IN1 - Right Motor Forward
#define RMB 4   // IN2 - Right Motor Backward
#define RMS 6   // ENA - Right Motor Speed PWM

// Left motor
#define LMF 3   // IN3 - Left Motor Forward
#define LMB 2   // IN4 - Left Motor Backward
#define LMS 5   // ENB - Left Motor Speed PWM


// ============================================================================
// Tuning Parameters
// ============================================================================

// PID values.
// Start with KI = 0. Increase KP and KD carefully during testing.
const float KP = 45.0;
const float KI = 0.0;
const float KD = 25.0;

// Motor speed settings.
// Values must be between 0 and 255.
const int BASE_SPEED = 170;
const int MAX_SPEED  = 255;

// Analog threshold.
// Sensor readings above this value are treated as "line detected".
// Adjust this after testing your sensor on black and white surfaces.
const int THRESHOLD = 500;


// ============================================================================
// Global Variables
// ============================================================================

float lastError = 0.0;
float integral  = 0.0;

// Weight values for each sensor from left to right.
const int SENSOR_WEIGHTS[5] = { -2, -1, 0, 1, 2 };


// ============================================================================
// Setup
// ============================================================================

void setup() {
  pinMode(RMF, OUTPUT);
  pinMode(RMB, OUTPUT);
  pinMode(RMS, OUTPUT);

  pinMode(LMF, OUTPUT);
  pinMode(LMB, OUTPUT);
  pinMode(LMS, OUTPUT);

  stopMotors();

  // Short delay before starting, useful when placing the robot on the track.
  delay(1500);
}


// ============================================================================
// Main Loop
// ============================================================================

void loop() {
  int sensorValues[5];

  readSensors(sensorValues);

  // If the line is completely lost, stop the robot for safety.
  if (isLineLost(sensorValues)) {
    integral = 0.0;
    stopMotors();
    return;
  }

  float error = calculateLineError(sensorValues);

  float correction = calculatePIDCorrection(error);

  // Motor polarity is reversed.
  // Negative speed values move the robot forward.
  int leftSpeed  = -(BASE_SPEED + (int)correction);
  int rightSpeed = -(BASE_SPEED - (int)correction);

  setMotors(leftSpeed, rightSpeed);
}


// ============================================================================
// Sensor Reading
// ============================================================================

void readSensors(int sensorValues[]) {
  sensorValues[0] = analogRead(S1);
  sensorValues[1] = analogRead(S2);
  sensorValues[2] = analogRead(S3);
  sensorValues[3] = analogRead(S4);
  sensorValues[4] = analogRead(S5);
}


// ============================================================================
// Line Error Calculation
// ============================================================================

float calculateLineError(int sensorValues[]) {
  int weightedSum = 0;
  int activeSensorCount = 0;

  for (int i = 0; i < 5; i++) {
    if (sensorValues[i] > THRESHOLD) {
      weightedSum += SENSOR_WEIGHTS[i];
      activeSensorCount++;
    }
  }

  // If no sensor is active, keep the last known direction.
  if (activeSensorCount == 0) {
    return lastError;
  }

  return (float)weightedSum / activeSensorCount;
}


// ============================================================================
// PID Controller
// ============================================================================

float calculatePIDCorrection(float error) {
  integral += error;

  float derivative = error - lastError;
  lastError = error;

  float correction = (KP * error) + (KI * integral) + (KD * derivative);

  return correction;
}


// ============================================================================
// Line Lost Detection
// ============================================================================

bool isLineLost(int sensorValues[]) {
  for (int i = 0; i < 5; i++) {
    if (sensorValues[i] > THRESHOLD) {
      return false;
    }
  }

  return true;
}


// ============================================================================
// Motor Control
// ============================================================================

void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed  = constrain(leftSpeed,  -MAX_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

  setLeftMotor(leftSpeed);
  setRightMotor(rightSpeed);
}


void setLeftMotor(int speedValue) {
  if (speedValue > 0) {
    digitalWrite(LMF, HIGH);
    digitalWrite(LMB, LOW);
    analogWrite(LMS, speedValue);
  }
  else if (speedValue < 0) {
    digitalWrite(LMF, LOW);
    digitalWrite(LMB, HIGH);
    analogWrite(LMS, -speedValue);
  }
  else {
    digitalWrite(LMF, LOW);
    digitalWrite(LMB, LOW);
    analogWrite(LMS, 0);
  }
}


void setRightMotor(int speedValue) {
  if (speedValue > 0) {
    digitalWrite(RMF, HIGH);
    digitalWrite(RMB, LOW);
    analogWrite(RMS, speedValue);
  }
  else if (speedValue < 0) {
    digitalWrite(RMF, LOW);
    digitalWrite(RMB, HIGH);
    analogWrite(RMS, -speedValue);
  }
  else {
    digitalWrite(RMF, LOW);
    digitalWrite(RMB, LOW);
    analogWrite(RMS, 0);
  }
}


void stopMotors() {
  digitalWrite(LMF, LOW);
  digitalWrite(LMB, LOW);
  analogWrite(LMS, 0);

  digitalWrite(RMF, LOW);
  digitalWrite(RMB, LOW);
  analogWrite(RMS, 0);
}