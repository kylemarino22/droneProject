#ifndef communication_h_
#define communication_h_
#include <string.h>
#include <stdio.h>

extern int throttle;
extern int roll;
extern int pitch;
extern int yaw;
int decodeData(char response []);

int decodeByte(char response []);



#endif
