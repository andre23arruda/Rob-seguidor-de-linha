void setup() {
Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0){
    char x = Serial.read();
    Serial.write(x);
  }

}
