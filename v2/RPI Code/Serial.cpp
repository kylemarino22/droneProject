#include "Serial.h"
#include <pigpio.h>
#include <stdio.h>

using namespace std;

/*
Class that handles serial communication for Raspberry PI
*/
class SerialHandler {
        //Serial Handle being used
        int handle;
        //Set once class has been initialized
        bool initFlag;

public:
        SerialHandler(bool initFlag) : {initFlag = false}; 
        void serialBegin();
        char * testSerial(); 
        void sendSerial();

};

/*
Initializes the serial handle
*/
void serialBegin(){
        char *serialHandle = "/dev/ttyACM1";
        handle = serOpen(serialHandle,115200,0);
        printf("handle %d", handle);
        if(handle < -1) {
                serialHandle = "/dev/ttyACM0";
                handle = serOpen(serialHandle,115200,0);
        }
}

/*
Reads the buffer; Used for Testing
*/
char * testSerial(){
        sendSerial('r', 0);
        delay(1);
        int bytestoRead = serDataAvailable(handle);
        static char readBuf[256];
        if(serDataAvailable(handle) > 0) {
                serRead(handle, readBuf, bytestoRead);
        }
        return readBuf;
}

/*
Sends serial data
*/
void sendSerial(uint8_t command, uint8_t data){
        serWriteByte(handle, command);
        serWriteByte(handle, data);
}

