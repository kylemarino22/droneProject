
#include "opticalEncoder.h"

OpticalEncoder::OpticalEncoder () {}

OpticalEncoder::setup (uint8_t irq_pin, void (*ISR_callback)(void), int value) {
    pinMode(irq_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(irq_pin), ISR_callback, value);
}

void OpticalEncoder::read (void)
{
    microsBuffer[microsBufferPos] = micros();
    microsBufferPos++;

    if (microsBufferPos >= BUFFER_LENGTH + 1) {
        doProcessBuffer = true;
    }
}

bool OpticalEncoder::processBuffer () {
    if (!doProcessBuffer) return false;
//    noInterrupts();

    noInterrupts();

    for (int i = 0; i < BUFFER_LENGTH; i++) {
        times[i] = microsBuffer[i+1] - microsBuffer[i];
    }


    uint32_t a;
    for (int i = 0; i < BUFFER_LENGTH; ++i) {
        for (int j = i + 1; j < BUFFER_LENGTH; ++j) {
            if (times[i] > times[j]) {
                a =  times[i];
                times[i] = times[j];
                times[j] = a;
            }
        }
    }

    speed = 0;

    for (int i = DATA_PADDING; i < BUFFER_LENGTH - DATA_PADDING; i++) {
        speed += times[i];
    }

    speed /= (BUFFER_LENGTH - 2 * DATA_PADDING);

    speed = 0.125 / ((double) speed / 1000000);

    microsBufferPos = 0;
    doProcessBuffer = false;

    interrupts();
    return true;
}
