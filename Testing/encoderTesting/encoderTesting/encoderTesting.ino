#include <OpticalEncoder.h>

OpticalEncoder * encoder1;


void setup() {
  Serial.begin(115200);
  encoder1 = new OpticalEncoder();
  encoder1->setup(2);
//  pinMode(2, INPUT);
//  attachInterrupt(digitalPinToInterrupt(2), test, CHANGE);

}

void test() {
  count ++;
}

void loop() {

//  encoder1.read();
  encoder1->processBuffer();
//  Serial.print(digitalRead(4));
//  Serial.print("         ");
//  Serial.println(count);
  Serial.println(encoder1->speed);
  delay(1000);
}
