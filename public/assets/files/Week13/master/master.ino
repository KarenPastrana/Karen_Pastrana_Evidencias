#include <Wire.h>
const int pushButton = D9;
void setup() {
  Wire.begin();
  pinMode(pushButton, INPUT);
  Serial.begin(9600);  // Initiates serial communication for debugging
  pinMode(D10, OUTPUT);
} 
void loop() {
  digitalWrite(D10, HIGH); //To be shure it works
  int buttonState = digitalRead(pushButton);  // Sends '0' to slave 1 if the button is not pressed
  if (buttonState == LOW) {
    Wire.beginTransmission(8);  // Address of slave 1
    Wire.write('0');
    Wire.endTransmission();
    Serial.println("Button not pressed - Signal sent to slave 1: 0");
    Wire.beginTransmission(9);  // Address of slave 1
    Wire.write('1');
    Wire.endTransmission();
  } else {// Sends '1' to slave 2 if the button is pressed
    Wire.beginTransmission(9);  // Address of slave 2
    Wire.write('0');
    Wire.endTransmission();
    Wire.beginTransmission(8);  // Address of slave 2
    Wire.write('1');
    Wire.endTransmission();
    Serial.println("Button pressed - Signal sent to slave 2: 1");
  }
  delay(100);  // Small delay to avoid button bounce
}


