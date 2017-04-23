#ifndef GYRO_H_
#define GYRO_H_

#define gyro_chipId (0x58)
#define gyro_Address (0x6B)

int gyro_fd;

typedef struct {
    float x;
    float y;
    float z;
}gyro_data;

gyro_data _gyro_data;

#endif
