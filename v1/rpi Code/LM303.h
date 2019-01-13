#ifndef LM303_H_
#define LM303_H_

#define LM303_mag_Address (0x1E)
#define LM303_accel_Address (0x19)
#include<stdint.h>
#include "struct.h"

int LM303_mag_fd;
int LM303_accel_fd;


XYZ accel_data;
XYZ mag_data;



int LM303_mag_begin();
int LM303_accel_begin();
void LM303_mag_read();
void LM303_accel_read();

// void fusionCalc();


#endif
