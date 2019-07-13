#include <OpticalEncoder.h>

OpticalEncoder * encoder1;

void setup() {
  Serial.begin(115200);
  encoder1 = new OpticalEncoder();
  encoder1->setup(2, []{encoder1->read();}, CHANGE);

}
void loop() {

//  encoder1.read();
  encoder1->processBuffer();
//  Serial.println(encoder1->speed);
//  delay(1000);
}
