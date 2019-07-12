//
// Created by Kyle on 2019-07-11.
//

#ifndef ENCODERTESTING_ENCODERSERIALTX_H
#define ENCODERTESTING_ENCODERSERIALTX_H

#include <SoftwareSerial.h>
#include "Arduino.h"


void sendSerialTX (SoftwareSerial & mySerial, uint8_t deviceID, uint16_t data1, uint16_t data2);

#endif //ENCODERTESTING_ENCODERSERIALTX_H
