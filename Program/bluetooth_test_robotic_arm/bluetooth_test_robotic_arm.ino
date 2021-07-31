#include <EEPROM.h>
#include <VarSpeedServo.h>

//Declaração dos pinos do servo
#define pinServoBase     2
#define pinServoAvanco   3
#define pinServoElevacao 4
#define pinServoGarra    5

//Declaração dos pinos do Joystick
#define potBaseVrx     A7
#define potAvancoVry   A6
#define potGarraVrx    A5
#define potElevacaoVry A4

#define pinBotao1 10
#define pinBotao2 11

VarSpeedServo servoBase, servoAvanco, servoElevacao, servoGarra;
#define tempo 50
// Variáveis para o cálculo dos ângulos
int anguloBase = 90, anguloAvanco = 90, anguloElevacao = 90, anguloGarra = 90;

void setup()
{
  Serial.begin(9600);

  //Configuração das portas dos servosb
  servoBase.attach(pinServoBase);
  servoAvanco.attach(pinServoAvanco);
  servoElevacao.attach(pinServoElevacao);
  servoGarra.attach(pinServoGarra);

  pinMode(pinBotao1, INPUT_PULLUP);
  pinMode(pinBotao2, INPUT_PULLUP);
}

void loop()
{
  if (Serial.available() > 0) {
    char comando = Serial.read();

    Serial.println(anguloBase);
    if (comando == 'b') {
      while (comando) {
        anguloBase = anguloBase - 1;
        servoBase.write(anguloBase, tempo);
        if (anguloBase == 0) break;
      }
    }
    else if (comando == 'B') {
      while (comando) {
        anguloBase = anguloBase + 1;
        servoBase.write(anguloBase, tempo);
        if (anguloBase == 180) break;
      }
    }

    else if (comando == 'a') {
      while (comando) {
        anguloAvanco = anguloAvanco - 1;
        servoAvanco.write(anguloAvanco, tempo);
        if (anguloAvanco == 0) break;
      }
    }
    else if (comando == 'A') {
      while (comando) {
        anguloAvanco = anguloAvanco + 10;
        servoAvanco.write(anguloAvanco, tempo);
        if (anguloAvanco == 180) break;
      }
    }

    else if (comando == 'e') {
      while (comando) {
        anguloElevacao = anguloElevacao - 1;
        servoElevacao.write(anguloElevacao, tempo);
        if (anguloElevacao == 0) break;
      }
    }
    else if (comando == 'E') {
      while (comando) {
        anguloElevacao = anguloElevacao + 1;
        servoElevacao.write(anguloElevacao, tempo);
        if (anguloElevacao == 180) break;
      }
    }

    else if (comando == 'g') {
      while (comando) {
        anguloGarra = anguloGarra - 1;
        servoGarra.write(anguloGarra, tempo);
        if (anguloGarra == 0) break;
      }
    }
    else if (comando == 'G') {
      while (comando) {
        anguloGarra = anguloGarra + 1;
        servoGarra.write(anguloGarra, tempo);
        if (anguloGarra == 180) break;
      }
    }
  }
}
