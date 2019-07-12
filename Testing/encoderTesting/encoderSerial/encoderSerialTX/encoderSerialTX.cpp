//
// Created by Kyle on 2019-07-11.
//

#include "encoderSerialTX.h"

uint8_t tx_byte_0;
uint8_t tx_byte_1;
uint8_t tx_byte_2;

void sendSerialTX (SoftwareSerial & mySerial, uint8_t deviceID, uint16_t data1, uint16_t data2) {

    tx_byte_0 = (deviceID << 4) | (data1 >> 6);
    tx_byte_1 = ((uint8_t) data1 << 2) | (data2 >> 8);
    tx_byte_2 = (uint8_t) data2;

    mySerial.write(0b01010101);
    mySerial.write(tx_byte_0);
    mySerial.write(tx_byte_1);
    mySerial.write(tx_byte_2);

}