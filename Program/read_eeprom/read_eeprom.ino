#include <EEPROM.h>
void setup() {
  Serial.begin(9600);
  for (byte x = 0; x < 255; x++) {
    EEPROM.write(x, 0);
  }
  for (byte x = 0; x < 255; x++) {
    Serial.println(EEPROM.read(x));
  }
}

void loop() {

}
