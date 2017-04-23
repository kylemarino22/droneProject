#ifndef LM303_H_
#define LM303_H_

#define LM303_mag_Address (0x1E)
#define LM303_accel_Address (0x19)
#include<stdint.h>

int LM303_mag_fd;
int LM303_accel_fd;

typedef struct{
    float x;
    float y;
    float z;

}accel_data;

typedef struct{
    float x;
    float y;
    float z;

}mag_data;

typedef struct{
    float roll;
    float pitch;
    float heading;
    
}fusion_data;

mag_data _mag_data;
accel_data _accel_data;
fusion_data _fusion_data;

int LM303_mag_begin();
int LM303_accel_begin();
void LM303_mag_read();
void LM303_accel_read();

void fusionCalc();
float meanTrimmer(float input[20]);
float average(float input[40]);

float headingCorrect (float finalheading);
float rollCorrect (float rollArray[40]);
float pitchCorrect (float pitchArray[40]);

#endif
