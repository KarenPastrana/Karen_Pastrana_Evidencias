#include <Wire.h>
const int pinLed = 3;
void setup() {
  Wire.begin(9); // Address of slave 1
  Wire.onReceive(receiveEvent); 
  pinMode(pinLed, OUTPUT);
}
void loop() {
    pinMode(4, OUTPUT); //To be shure it works
}
void receiveEvent(int howMany) {
  digitalWrite(4, HIGH);
  while (Wire.available()) {
    char data = Wire.read();
    
    // Controls the LED of slave 1
    if (data == '0') {
      digitalWrite(pinLed, HIGH);
    } else {
      digitalWrite(pinLed, LOW);
    }
  }
}


