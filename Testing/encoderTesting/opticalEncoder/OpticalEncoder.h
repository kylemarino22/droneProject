
#ifndef _opticalEncoder_h_
#define _opticalEncoder_h_

#include "Arduino.h"

#define BUFFER_LENGTH 100
#define DATA_PADDING 40


class OpticalEncoder {
private:
//    bool pin1_Switch = false;
//    bool pin1_ON = false;
//    long delta_t = 0;
    bool doProcessBuffer = false;
    long init_time = micros();
    long microsBuffer[BUFFER_LENGTH + 1];
    long times[BUFFER_LENGTH];

//    int bufferLength = 10;
    int microsBufferPos = 0;
//    int maxValue;
//    int minValue;
//    int maxValuePos;
//    int minValuePos;

public:
    float speed;
    OpticalEncoder();
    setup (uint8_t irq_pin, void (*ISR_callback)(void), int value);
    void read (void);
    bool processBuffer();

};



#endif
