#include <encoderSerialTX.h>

//Must be Different pins
SoftwareSerial mySerial(4, 5); // RX, TX

uint16_t Data1 = 999;
uint16_t Data2 = 25;
uint8_t deviceID = 1;

void setup() {
  Serial.begin(115200);
  mySerial.begin(74880);
}

void loop() {

  sendSerialTX(mySerial, deviceID, Data1, Data2);
  delay(1);
  
}
