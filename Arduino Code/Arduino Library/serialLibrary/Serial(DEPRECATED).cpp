#include "Serial.h"
#include <Arduino.h>

void beginPin(serialPin *Spin, int min, int max, int pin, char pinType){
        Spin->min = min;
        Spin->max = max;
        Spin->pin = pin;
        if(pinType == 'O'){
          pinMode(pin, OUTPUT);
        }
        else if (pinType == 'I'){
          pinMode(pin,INPUT);
        }
        else{
          Serial.println("Invalid pin type");
        }
}

void writePin(serialPin *Spin, int val){
        int pwm = (int)((analogAccuracy / (Spin->max - Spin->min))*(-Spin->min + val));
        analogWrite(Spin->pin, pwm);
}

int readPin(serialPin *Spin){
        int val = digitalRead(Spin->pin);
        bool isUp = false;
        while (val == 1) {
                val = digitalRead(Spin->pin);
                isUp = true;
        }
        if (isUp == true) {
                int start = micros();
                while (val == 0) {
                        val = digitalRead(Spin->pin);
                }
                int dtime = micros() - start;
                int output = (int)((float)(analogReadAccuracy - dtime) * ((float)(Spin->max - Spin->min)/
                (float)analogReadAccuracy) - (float)(-Spin->min + 0.5)); // The 0.5 is added for rounding
                return output;
        }
}
//Serial Decode compares up to 3 reads from the bus to increase
//the accuracy of what is being read.
int serialDecode(serialPin *Spin){
  int read1 = readPin(Spin);
  int read2 = readPin(Spin);
  if(read1 == read2) return read1;
  int read3 = readPin(Spin);
  if(read1 == read3) return read1;
  if(read2 == read3) return read2;
  return (read1 + read2 + read3)/3;
}
