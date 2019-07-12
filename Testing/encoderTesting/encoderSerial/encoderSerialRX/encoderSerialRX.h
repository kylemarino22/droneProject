//
// Created by Kyle on 2019-07-11.
//

#ifndef ENCODERTESTING_ENCODERSERIALRX_H
#define ENCODERTESTING_ENCODERSERIALRX_H

#include <SoftwareSerial.h>
#include "Arduino.h"

extern uint8_t deviceID;
extern uint16_t data1rx;
extern uint16_t data2rx;

bool readSerialRX (SoftwareSerial & mySerial);

#endif //ENCODERTESTING_ENCODERSERIALRX_H
