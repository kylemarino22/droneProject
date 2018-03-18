
#ifndef _sensors_h_
#define _sensors_h_

#include <Kalman.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>

typedef struct XYZ {
        float x, y, z;
} XYZ;


extern Adafruit_LSM303_Accel_Unified accel;
extern Adafruit_LSM303_Mag_Unified mag;
extern Adafruit_L3GD20 gyro;

extern XYZ kalAngle;
extern XYZ gyroAngle;
extern XYZ gyro_calibrate;
extern XYZ accelXYZ;
extern XYZ magXYZ;
extern XYZ gyroXYZ;
extern XYZ gyroRate;
extern XYZ current;
extern XYZ accel_calibrate;

extern Kalman kalmanX;
extern Kalman kalmanY;

extern bool waitOnce;

extern long long currentRead, lastRead;

void calibrateGyro();
void calibrateAccel();

void beginSensors(boolean);

void computeRoll(XYZ*);

void computePitch(float, XYZ*);

void calculateGyro(float dt);

float calculateTime();

void calculateFusion();
void calculateFusion2();




#endif
