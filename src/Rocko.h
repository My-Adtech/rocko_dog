/*
  Rocko.h - Library for the STEM robot dog Rocko.
  Created by MYADTECH.
  Engineers Marco Andrade & Rodrigo Villanueva, November 04, 2021.
*/
#ifndef Rocko_h
#define Rocko_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
#include "math.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

//This values are for SG90, change values according to your servos
#define SERVOMIN  200 // This is the 'minimum' pulse length count (out of 4096)= 0°
#define SERVOMAX  577 // This is the 'maximum' pulse length count (out of 4096)= 180°
#define USMIN  500 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

class Rocko
{
  public:
    Rocko();
    void begin();
    void Up();
    void Down();
    
    // Gait Control
    void MoveLeg(int leg, float x, float z);
    void setLegPosition(int leg, float x, float z);
    void Trot();
    void Walk();
    
    // IMU
    void initIMU();
    void stabilize();
    
    // Calibration
    void setTrim(int leg, int joint, int value);
    
  private:
    Adafruit_PWMServoDriver _pwm;
    Adafruit_MPU6050 _imu;

    // Servo limits
    int _SERVOMIN;
    int _SERVOMAX;
    int _trim[8] = {0}; // Trim values for 8 servos

    // Robot Dimensions (cm)
    float femur = 12.0;
    float calf = 12.0;  // cm

    // Servo Mapping (2 servos per leg: Hip, Knee)
    // Leg 0: FL, Leg 1: FR, Leg 2: BL, Leg 3: BR
    const int HIP_PINS[4] = {0, 2, 4, 6};
    const int KNEE_PINS[4] = {1, 3, 5, 7};

    // Helper methods
    void WriteServo(int servonum, int angleValue);
    float GetAlphaAng(float x, float z);
    float GetBetaAng(float x, float z);
    float GetGammaAng(float x, float z);
};

#endif
