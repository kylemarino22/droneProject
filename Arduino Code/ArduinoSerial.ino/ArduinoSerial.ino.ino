
int throttle;
int roll;
int yaw;
int pitch;
void setup() {
  Serial.begin(115200);

}

void loop() {
  if (Serial.available() > 1) {
                // read the incoming byte:
                int incomingByte = Serial.read();
                int incomingData = Serial.read();

                if(incomingByte == 'a'){

                  throttle = incomingData;
                }
                if(incomingByte == 'b'){

                  roll = incomingData;
                }
                if(incomingByte == 'c'){

                  pitch = incomingData;
                }
                if(incomingByte == 'd'){

                  yaw = incomingData;
                }
                if(incomingByte == 'r'){
                  Serial.print((char)throttle); Serial.print("\t");
                  Serial.print((char)roll); Serial.print("\t");
                  Serial.print((char)pitch); Serial.print("\t");
                  Serial.print((char)yaw);
                }
                if(incomingByte == 'q'){
                  Serial.println("quit");
                  while(1);
                }
        }

        
}
