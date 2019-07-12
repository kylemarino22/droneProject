#include "Serial.h"
#include <pigpio.h>
#include <stdio.h>

using namespace sh;
using namespace std;

/*
Initializes the serial handle
*/
void SerialHandler::serialBegin(){

        printf("hello\n");
        // char *serialHandle = "/dev/ttyACM1";
        // handle = serOpen(serialHandle,115200,0);
        // printf("handle %d", handle);
        // if(handle < -1) {
        //         serialHandle = "/dev/ttyACM0";
        //         handle = serOpen(serialHandle,115200,0);
        // }
}

// /*
// Reads the buffer; Used for Testing
// */
// char * SerialHandler::testSerial(){
//         this->sendSerial('r', 0);

//         int bytestoRead = serDataAvailable(handle);
//         static char readBuf[256];
//         if(serDataAvailable(handle) > 0) {
//                 serRead(handle, readBuf, bytestoRead);
//         }
//         return readBuf;
// }

// /*
// Sends serial data
// */
// void SerialHandler::sendSerial(uint8_t command, uint8_t data){
//         serWriteByte(handle, command);
//         serWriteByte(handle, data);
// }

