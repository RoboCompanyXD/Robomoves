#include <SoftwareSerial.h>



// Definitions
#define rxPin 3
#define txPin 4

//SoftwareSerial mySerial(rxPin, txPin);
SoftwareSerial  mySerial(rxPin, txPin);


void setup() {

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  
  mySerial.begin(115200);

  mySerial.write(7);

  delay(10000);
  
  mySerial.write(128);

  delay(100);

  mySerial.write(132);
  
  delay(100);

  mySerial.write(138);

  delay(100);

  mySerial.write(4);

}


void loop() {

    digitalWrite(13,LOW);
    delay(250);
    digitalWrite(13,HIGH);
    delay(250);
    


}
