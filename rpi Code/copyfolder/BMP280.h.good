#ifndef BMP280_H_
#define BMP280_H_

#define BMP280_ChipID (0x58)
#define BMP_Address (0x77)
#include<stdint.h>

int32_t BMP_tfine;
float BMP_finalP;
int BMP_fd;


int BMP280_begin();
void readData();
float readAltitude(float seaLevelPa);
float readPressure();
float readTemperature();

typedef struct{

uint16_t t1;
int16_t t2;
int16_t t3;

uint16_t p1;
int16_t p2;
int16_t p3;
int16_t p4;
int16_t p5;
int16_t p6;
int16_t p7;
int16_t p8;
int16_t p9;

int32_t adcP;
int32_t adcT;

}BMP_calib_Data;


BMP_calib_Data _BMP_calib_Data;


#endif
