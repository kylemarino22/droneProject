#ifndef Serial_h_
#define Serial_h_

const int analogAccuracy = 255;
const int analogReadAccuracy = 1260;

typedef struct serialPin {
  int min, max;
  int pin;

} serialPin;

void beginPin(serialPin*, int min, int max, int pin, char pinType);

void writePin(serialPin*, int val);
int readPin(serialPin*);
int serialDecode(serialPin*);
#endif
