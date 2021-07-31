void setup() {
  Serial.begin(9600);

  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
}

void loop() {
  Serial.println(digitalRead(9));
  Serial.println(digitalRead(8));
  Serial.println(digitalRead(7));
  Serial.println(digitalRead(6));
  Serial.println("*****");
  delay(100);
}
