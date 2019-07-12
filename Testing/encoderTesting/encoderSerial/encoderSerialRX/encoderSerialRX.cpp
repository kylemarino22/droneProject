//
// Created by Kyle on 2019-07-11.
//
#include "encoderSerialRX.h"

uint8_t deviceID;
uint16_t data1rx;
uint16_t data2rx;

uint8_t rx_byte_0;
uint16_t rx_byte_1;
uint16_t rx_byte_2;

bool readSerialRX (SoftwareSerial & mySerial) {

    while (mySerial.available() > 3) {

        if (mySerial.read() != 0b01010101) {
            //Framing byte missed,
            mySerial.flush();
            break;
        }
        rx_byte_0 = mySerial.read();
        rx_byte_1 = mySerial.read();
        rx_byte_2 = mySerial.read();

        deviceID = rx_byte_0 >> 4;
        data1rx = ((rx_byte_0 << 6) | (rx_byte_1 >> 2)) & (0b0000001111111111);
        data2rx = ((rx_byte_1 << 8) | (rx_byte_2)) & (0b0000001111111111);
        return true;
    }
    return false;
}