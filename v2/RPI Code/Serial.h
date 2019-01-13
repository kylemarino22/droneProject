#ifndef Serial_h_
#define Serial_h_
#include <stdint.h>

extern int handle;
extern int initFlag;

//void sendData(uint8_t command, uint8_t data);
void sendDatav2(uint8_t command, uint8_t data);
char* testSerial();
int* serialDecoder(char* readBuf);
void sendSerial(uint8_t command, uint8_t data);
void serialBegin();
void a();

#endif
