#include "../../encoderSerial/encoderSerialRX/encoderSerialRX.h"

//Can be the same pin
SoftwareSerial mySerial(4, 4); // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(74880);
}

void loop() {

  if(readSerialRX(mySerial)) {
    Serial.println(deviceID);
    Serial.println(data1rx);
    Serial.println(data2rx);
  }

}
