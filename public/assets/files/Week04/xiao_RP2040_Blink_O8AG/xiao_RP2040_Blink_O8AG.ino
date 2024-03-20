
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D1, INPUT);
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  pinMode(D7, OUTPUT);
  digitalWrite(26, HIGH);
  pinMode(D6, OUTPUT);
  digitalWrite(26, HIGH);

  //pinMode(PIN_LED_R, OUTPUT);
  //pinMode(PIN_LED_G, OUTPUT);
  //pinMode(PIN_LED_B, OUTPUT);
  //digitalWrite(PIN_LED_R, HIGH);
  //digitalWrite(PIN_LED_G, HIGH);
  //digitalWrite(PIN_LED_B, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  /*digitalWrite(PIN_LED_B, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(PIN_LED_B, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);*/                      // wait for a second

  digitalWrite(26, digitalRead(D1));  // turn the LED on (HIGH is the voltage level)
  digitalWrite(D6, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(D7, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(26, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(D6, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(D7, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);

}