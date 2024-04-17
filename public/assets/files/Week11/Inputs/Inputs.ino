//Change the pin here according to your connection
const int touch_pin = 8;
const int led_pin =9;
 
void setup()
{
  Serial.begin(9600);
 //define the touch pin as an input pin
  pinMode(touch_pin, INPUT);
  pinMode(led_pin, OUTPUT);
}
 
//read the button status and display the message once touch is detected
void loop()
{
  digitalWrite(led_pin, HIGH);
  if (digitalRead(touch_pin) == HIGH)
  {
    digitalWrite(9, HIGH);
    //Serial.println("Touch detected!");
    delay(100);
  }
  else{
    digitalWrite(9, LOW);
    //Serial.println("Toch not detected");
  }
  delay(1000);
} 