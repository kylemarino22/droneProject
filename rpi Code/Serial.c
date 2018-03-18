#include "Serial.h"
#include <pigpio.h>
#include <stdio.h>
int handle;
int initFlag = 0;

char * testSerial(){
        //serWriteByte(handle,'a');
        sendSerial('r', 0);
        delay(1);
        int bytestoRead = serDataAvailable(handle);
        static char readBuf[256];
        if(serDataAvailable(handle) > 0) {
                serRead(handle, readBuf, bytestoRead);
        }
        return readBuf;
}

int* serialDecoder(char* readBuf){
        static int output[4];


        output[0] = readBuf[0];
        output[1] = readBuf[2];
        output[2] = readBuf[4];
        output[3] = readBuf[6];

        return output;
}

void sendSerial(uint8_t command, uint8_t data){
        serWriteByte(handle, command);
        serWriteByte(handle, data);
}

void serialBegin(){
        char *serialHandle = "/dev/ttyACM1";
        handle = serOpen(serialHandle,115200,0);
        printf("handle %d", handle);
        if(handle < -1) {
                serialHandle = "/dev/ttyACM0";
                handle = serOpen(serialHandle,115200,0);
        }

}
void sendDatav2(uint8_t command, uint8_t data){
        int i;
        int j;
        uint8_t controlByte;
        uint8_t byteToSend;
        gpioWrite(27,1); //start
        delayMicroseconds(75);
        for(j = 0; j < 2; j++) {

                delayMicroseconds(50);
                gpioWrite(27,1);
                // printf("on\n");
                delayMicroseconds(100);
                gpioWrite(27,0);
                delayMicroseconds(75);

                if(j == 0) {
                        // Command
                        controlByte = 1;
                        byteToSend = command;
                }
                if(j == 1) {
                        // Data
                        controlByte = 0;
                        byteToSend = data;
                }

                // gpioWrite(27,(controlByte & ( 1 << 1)) >> 1); //data
                // delayMicroseconds(1000);
                // gpioWrite(27,(controlByte & ( 1 << 0)) >> 0);
                // delayMicroseconds(1000);

                for(i = 3; i >=0; i--) {

                        gpioWrite(27,(byteToSend & ( 1 << ((i*2) + 1))) >> ((i*2)+1));//data
                        delayMicroseconds(75);
                        gpioWrite(27,(byteToSend & ( 1 << (i*2))) >> (i*2)); // data
                        delayMicroseconds(75);

                }

                gpioWrite(27,0);

                // if(j == 0) {
                //         // printf("off\n");
                //         delayMicroseconds(50);
                //         gpioWrite(27,1);
                //         // printf("on\n");
                //         delayMicroseconds(500);
                //         gpioWrite(27,0);
                //         // printf("off\n");
                // }

        }

}

// void sendData(uint8_t command, uint8_t data){
//         int i;
//         int j;
//         uint8_t controlByte;
//         uint8_t byteToSend;
//         for(j = 0; j < 2; j++) {
//
//                 if(j == 0) {
//                         // Command
//                         controlByte = 1;
//                         byteToSend = command;
//                 }
//                 if(j == 1) {
//                         // Data
//                         controlByte = 0;
//                         byteToSend = data;
//                 }
//                 // gpioWrite(27,0); // Start of write cycle
//                 // delay(10);
//
//
//                 //Control Byte Send
//                 gpioWrite(27,(controlByte & ( 1 << 1)) >> 1);//data
//                 gpioWrite(17,1);//clock
//                 delayMicroseconds(1000);
//                 gpioWrite(27,0);//data
//
//                 gpioWrite(27,(controlByte & ( 1 << 0)) >> 0);
//                 gpioWrite(17,0);
//                 delayMicroseconds(1000);
//                 gpioWrite(27,0);
//
//                 //DataByte send
//                 for(i = 3; i >=0; i--) {
//
//                         gpioWrite(27,(byteToSend & ( 1 << ((i*2) + 1))) >> ((i*2)+1));//data //
//                         gpioWrite(17,1);//clock                                              // Rising Edge
//                         delayMicroseconds(1000);                                                    //
//                         gpioWrite(27,0);//data                                               //
//
//                         gpioWrite(27,(byteToSend & ( 1 << (i*2))) >> (i*2)); // data         //
//                         gpioWrite(17,0);                                               // Falling Edge
//                         delayMicroseconds(1000);                                                  //
//                         gpioWrite(27,0);                                               //
//                 }
//
//                 // gpioWrite(27,1); // Write High to idle
//                 delayMicroseconds(5000);
//
//         }
// }
