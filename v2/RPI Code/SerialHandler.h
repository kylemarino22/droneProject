#ifndef SerialHandler_h_
#define SerialHandler_h_
#include <stdint.h>

extern int handle;
extern int initFlag;

//void sendData(uint8_t command, uint8_t data);
// void sendDatav2(uint8_t command, uint8_t data);
// char* testSerial();
// int* serialDecoder(char* readBuf);
// void sendSerial(uint8_t command, uint8_t data);

namespace sh
{
	/*
	Class that handles serial communication for Raspberry PI
	*/
	class SerialHandler {
	        //Serial Handle being used
	        int handle;
	        //Set once class has been initialized
	        bool initFlag;

	public:
	        SerialHandler(bool initFlag) {initFlag = false;} 
	        void serialBegin();
	        

	};
}


#endif
