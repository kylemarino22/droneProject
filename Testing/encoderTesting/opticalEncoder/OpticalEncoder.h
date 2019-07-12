
#ifndef _opticalEncoder_h_
#define _opticalEncoder_h_

#include "Arduino.h"

class OpticalEncoder {
private:
    bool pin1_Switch = false;
    bool pin1_ON = false;
    long delta_t = 0;
    long prev_time = micros();
    double circularBuffer[10];
    int bufferLength = 5;
    int pos = 0;
    int maxValue;
    int minValue;
    int maxValuePos;
    int minValuePos;

public:
    float speed;
    OpticalEncoder();
    OpticalEncoder(int pin);
    void read();

};




#endif
