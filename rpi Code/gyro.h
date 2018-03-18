#ifndef GYRO_H_
#define GYRO_H_
#include "struct.h"

#define gyro_chipId (0x58)
#define gyro_Address (0x6B)

int gyro_fd;

XYZ gyro_data;
XYZ gyro_calibrate;
XYZ gyro_rate;

void gyroCalibrate();
void gyroCalculate(float dt);

#endif
