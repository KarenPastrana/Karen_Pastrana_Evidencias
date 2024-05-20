#include <Wire.h>
const int pinLed = LED_BUILTIN;
void setup() {
  Wire.begin(8); // Address of slave 1
  Wire.onReceive(receiveEvent); 
  pinMode(pinLed, OUTPUT);
}
void loop() {
  // No need for code in the loop of a slave
}
void receiveEvent(int howMany) {
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


