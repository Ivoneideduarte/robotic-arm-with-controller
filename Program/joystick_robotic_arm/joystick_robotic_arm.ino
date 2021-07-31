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

VarSpeedServo servoBase, servoAvanco, servoElevacao, servoGarra;
#define tempo 120
// Variáveis para o cálculo dos ângulos
int anguloBase, anguloAvanco, anguloElevacao, anguloGarra;

void setup()
{
  Serial.begin(9600);

  //Configuração das portas dos servos
  servoBase.attach(pinServoBase);
  servoAvanco.attach(pinServoAvanco);
  servoElevacao.attach(pinServoElevacao);
  servoGarra.attach(pinServoGarra);
}

void loop()
{
  anguloBase = analogRead(potBaseVrx);
  anguloAvanco = analogRead(potAvancoVry);
  anguloElevacao = analogRead(potElevacaoVry);
  anguloGarra = analogRead(potGarraVrx);
  //Serial.println(anguloBase);

  anguloBase = map(anguloBase, 0, 1023, 0, 177);
  servoBase.write(anguloBase, tempo);

  anguloAvanco = map(anguloAvanco, 0, 1023, 40, 140);
  servoAvanco.write(anguloAvanco, tempo);

  anguloElevacao = map(anguloElevacao, 0, 1023, 0, 130);
  servoElevacao.write(anguloElevacao, tempo);

  anguloGarra = map(anguloGarra, 0, 1023, 90, 177);
  servoGarra.write(anguloGarra, tempo);


  //s1 = Base | 0 = Direita, 90 = frente, 177 = Esquerda
  //s2 = Avanço | 140 = Esticado, descanço = 90, 40 = Dobrado
  //s3 = Elevação | 130 = Cima, descanço = 90, 0 = Baixo
  //s4 = Garra | 90 = Aberta, 177 = Fechada
}
