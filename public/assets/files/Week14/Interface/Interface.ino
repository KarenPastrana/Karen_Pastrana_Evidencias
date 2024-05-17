void setup() {
  pinMode (D10, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if (Serial.available() > 0){
    char dato = Serial.read();
    if(dato == '1'){
      digitalWrite(D10, HIGH);
    }
    else if(dato == '0'){
      digitalWrite(D10, LOW);
    }
  }
}
