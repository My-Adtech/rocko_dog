#include "Arduino.h"
#include "Rocko.h"
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
#include "math.h"

Rocko::Rocko(){
  _SERVOMIN = SERVOMIN;
  _SERVOMAX = SERVOMAX;
}

void Rocko::begin(){
  _pwm = Adafruit_PWMServoDriver();
  _pwm.begin();
  _pwm.setOscillatorFrequency(27000000);
  _pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
  delay(10);
}

void Rocko::Up(){
  Serial.println("Rocko UP");
  for(int i=0; i<4; i++) MoveLeg(i, 0, 11); // Stand up (x=0, z=11)
}

void Rocko::Down(){
  Serial.println("Rocko Down");
  for(int i=0; i<4; i++) MoveLeg(i, 0, 7); // Sit down (x=0, z=7)
}

void Rocko::MoveLeg(int leg, float x, float z){
  if (leg < 0 || leg > 3) return;

  float alpha = GetAlphaAng(x, z); // Hip angle relative to vertical
  float beta  = GetBetaAng(x, z);  // Knee angle
  // Gamma is not strictly needed if we compute Alpha/Beta directly from X,Z
  
  // Convert to degrees
  int alphaDeg = round(alpha * 57.2957);
  int betaDeg = round(beta * 57.2957);
  
  // Adjust for servo mounting (assuming 90 is straight down/out)
  // This depends heavily on physical assembly. 
  // Assuming: 
  // Hip: 90 is vertical, <90 forward, >90 backward? 
  // Or Hip: 0 is forward, 180 backward.
  // Let's assume the IK returns angles inside the triangle.
  
  // For a simple 2DOF leg:
  // Theta (Hip) = atan2(x, z) + acos(...)
  // Phi (Knee) = acos(...)
  
  // Let's use the calculated angles directly for now, assuming standard configuration
  // We might need to invert angles for right side legs if servos are mirrored.
  
  int hipAngle = alphaDeg;
  int kneeAngle = betaDeg;
  
  // Mirroring for Right side (Legs 1 and 3) if needed
  // if (leg % 2 != 0) { ... } 

  WriteServo(HIP_PINS[leg], hipAngle);
  WriteServo(KNEE_PINS[leg], kneeAngle);
}

float Rocko::GetAlphaAng(float x, float z){
  float L = sqrt(x*x + z*z);
  // Angle of the leg line relative to vertical (or horizontal depending on frame)
  float theta = atan2(x, z); 
  // Angle inside the triangle at the hip
  float phi = acos((pow(femur, 2) + pow(L, 2) - pow(calf, 2)) / (2 * femur * L));
  return theta + phi; // Total hip angle
}

float Rocko::GetBetaAng(float x, float z){
  float L = sqrt(x*x + z*z);
  // Angle inside the triangle at the knee
  float beta = acos((pow(femur, 2) + pow(calf, 2) - pow(L, 2)) / (2 * femur * calf));
  return beta; 
}

float Rocko::GetGammaAng(float x, float z){
  return 0; // Not used in this 2D model
}

void Rocko::setLegPosition(int leg, float x, float z){
  MoveLeg(leg, x, z);
}

void Rocko::Trot(){
  // Simple Trot Gait: Diagonals move together
  // Phase 1: FL+BR lift and move forward, FR+BL move backward on ground
  float stepLen = 4.0;
  float liftHeight = 3.0;
  float standHeight = 11.0;
  
  // Lift FL (0) and BR (3)
  MoveLeg(0, stepLen/2, standHeight - liftHeight);
  MoveLeg(3, stepLen/2, standHeight - liftHeight);
  // Move FR (1) and BL (2) backward
  MoveLeg(1, -stepLen/2, standHeight);
  MoveLeg(2, -stepLen/2, standHeight);
  delay(200);
  
  // Place FL and BR
  MoveLeg(0, stepLen/2, standHeight);
  MoveLeg(3, stepLen/2, standHeight);
  delay(200);
  
  // Phase 2: FR+BL lift and move forward, FL+BR move backward
  // Lift FR (1) and BL (2)
  MoveLeg(1, stepLen/2, standHeight - liftHeight);
  MoveLeg(2, stepLen/2, standHeight - liftHeight);
  // Move FL (0) and BR (3) backward
  MoveLeg(0, -stepLen/2, standHeight);
  MoveLeg(3, -stepLen/2, standHeight);
  delay(200);
  
  // Place FR and BL
  MoveLeg(1, stepLen/2, standHeight);
  MoveLeg(2, stepLen/2, standHeight);
  delay(200);
}

void Rocko::Walk(){
  // Placeholder for Walk gait
  Trot(); 
}

void Rocko::initIMU(){
  if (!_imu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  } else {
    Serial.println("MPU6050 Found!");
    _imu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _imu.setGyroRange(MPU6050_RANGE_500_DEG);
    _imu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
}

void Rocko::stabilize(){
  sensors_event_t a, g, temp;
  _imu.getEvent(&a, &g, &temp);

  // PD Controller for stabilization
  // P (Proportional): Corrects based on current angle error (Approx from Accel)
  // D (Derivative): Dampens based on rate of change (Gyro)
  
  // Approximate Angle (Error) - assuming small angles
  float pitchError = a.acceleration.y; 
  float rollError = a.acceleration.x;
  
  // Angular Velocity (Derivative)
  float pitchRate = g.gyro.y;
  float rollRate = g.gyro.x;
  
  // Gains (Tune these!)
  float Kp = 0.5;
  float Kd = 0.1; 
  
  // Calculate Control Output
  float pitchOutput = (pitchError * Kp) + (pitchRate * Kd);
  float rollOutput = (rollError * Kp) + (rollRate * Kd);
  
  // Calculate offsets
  float frontOffset = pitchOutput;
  float backOffset = -pitchOutput;
  float leftOffset = rollOutput;
  float rightOffset = -rollOutput;
  
  // Apply to legs (Base height 11cm)
  // Leg 0 (FL): Front + Left
  MoveLeg(0, 0, 11 + frontOffset + leftOffset);
  // Leg 1 (FR): Front + Right
  MoveLeg(1, 0, 11 + frontOffset + rightOffset);
  // Leg 2 (BL): Back + Left
  MoveLeg(2, 0, 11 + backOffset + leftOffset);
  // Leg 3 (BR): Back + Right
  MoveLeg(3, 0, 11 + backOffset + rightOffset);
  
  delay(10); // Small delay for stability
}

void Rocko::setTrim(int leg, int joint, int value){
  // joint: 0 for Hip, 1 for Knee
  int servoIndex = -1;
  if (joint == 0) servoIndex = HIP_PINS[leg];
  else if (joint == 1) servoIndex = KNEE_PINS[leg];
  
  if (servoIndex != -1 && servoIndex < 8) {
    _trim[servoIndex] = value;
  }
}

void Rocko::WriteServo(int servonum, int angleValue){
  // Apply trim
  int trimmedAngle = angleValue + _trim[servonum];
  trimmedAngle = constrain(trimmedAngle, 0, 180);
  
  int pwmValue = map(trimmedAngle, 0, 180, _SERVOMIN, _SERVOMAX);
  _pwm.setPWM(servonum, 0, pwmValue);
}