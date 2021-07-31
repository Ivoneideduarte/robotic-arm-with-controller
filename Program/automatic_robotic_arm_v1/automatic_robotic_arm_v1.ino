//VarSpeedServo
#include <EEPROM.h>
#include <Servo.h>

#define potBase     A0
#define potAvanco   A1
#define potElevacao A2
#define potGarra    A3

#define pinBotaoReset  6 // Botão Vermelho: Reset
#define pinBotaoGravar 7 // Botão Verde: Gravar
#define pinBotaoAuto   8 // Botão Azul: Automático
#define pinBotaoManual 9 // Botão Amarelo: Manual

Servo servoBase, servoAvanco, servoElevacao, servoGarra;

int anguloBase, anguloAvanco, anguloElevacao,anguloGarra;
bool modoManual = false, modoAutomatico = false;
int x = 0;
byte add = 0;

void setup()
{
  Serial.begin(9600);

  servoBase.attach(2);
  servoAvanco.attach(3);
  servoElevacao.attach(4);
  servoGarra.attach(5);  

  pinMode(pinBotaoGravar, INPUT_PULLUP);
  pinMode(pinBotaoAuto, INPUT_PULLUP);
  pinMode(pinBotaoManual, INPUT_PULLUP);
  pinMode(pinBotaoReset, INPUT_PULLUP);
}

void loop() 
{
  if (!digitalRead(pinBotaoGravar)) {
    delay(200);
    gravar();
  }
  else if (!digitalRead(pinBotaoAuto)) {
    delay(250);
    modoAutomatico = true;
    automatico();
  }
  else if (!digitalRead(pinBotaoManual)) {
    delay(250);
    modoManual = true;
    manual();
  }
}

void gravar() 
{
  Serial.println("gravar");
  add = add + 1;
  EEPROM.write(add, anguloBase);
  add = add + 1;
  EEPROM.write(add, anguloGarra);
  add = add + 1;
  EEPROM.write(add, anguloAvanco);
  add = add + 1;
  EEPROM.write(add, anguloElevacao);
  delay(200);
}

void automatico() {
  Serial.println("automatico");
  byte ADD = 0;
  while (modoAutomatico) 
  {
    for (x = 0; x < 64; x++) 
    {
      ADD = ADD + 1;
      anguloBase = EEPROM.read(ADD);
      ADD = ADD + 1;
      anguloGarra = EEPROM.read(ADD);
      ADD = ADD + 1;
      anguloAvanco = EEPROM.read(ADD);
      ADD = ADD + 1;
      anguloElevacao = EEPROM.read(ADD);

      if (!digitalRead(pinBotaoReset)) {
        Serial.println("Resetando...");
        modoAutomatico = false;
      }

      if (anguloBase == 0) {
        break;
      }
      else {
        servoBase.write(anguloBase);
        servoGarra.write(anguloGarra);
        servoAvanco.write(anguloAvanco);
        servoElevacao.write(anguloElevacao);
      }
      delay(1000);
    }
    ADD = 0;
  }
}

void manual() 
{
  Serial.println("manual");
  while (modoManual) 
  {
    //Serial.println("ainda");
    anguloBase = analogRead(potBase);
    anguloElevacao = analogRead(potElevacao);
    anguloAvanco = analogRead(potAvanco);
    anguloGarra = analogRead(potGarra);

    anguloBase = map(anguloBase, 0, 1023, 6, 156);
    servoBase.write(anguloBase);

    anguloElevacao = map(anguloElevacao, 0, 1023, 0, 178);
    servoElevacao.write(anguloElevacao);

    anguloAvanco = map(anguloAvanco, 0, 1023, 0, 177);
    servoAvanco.write(anguloAvanco);

    anguloGarra = map(anguloGarra, 0, 1023, 10, 90);
    servoGarra.write(anguloGarra);

    if (!digitalRead(pinBotaoReset)) {
      Serial.println("Resetando...");
      delay(250);
      modoManual = false;
    }
    delay(1);
  }
}
