#include <Servo.h>

#define potBase A0
#define potAvanco A1
#define potElevacao A2
#define potGarra A3

Servo servoBase;
Servo servoAvanco;
Servo servoElevacao;
Servo servoGarra;

int anguloBase;
int anguloAvanco;
int anguloElevacao;
int anguloGarra;

int tmp = 1000;

void setup()
{
  servoBase.attach(2);
  servoAvanco.attach(3);
  servoElevacao.attach(4);
  servoGarra.attach(5);

  Serial.begin(9600);
}

void loop()
{
  anguloBase = analogRead(potBase);
  anguloBase = map(anguloBase, 0, 1023, 0, 180);
  servoBase.write(anguloBase);

  anguloAvanco = analogRead(potAvanco);
  anguloAvanco = map(anguloAvanco, 0, 1023, 0, 180);
  servoAvanco.write(anguloAvanco);

  anguloElevacao = analogRead(potElevacao);
  anguloElevacao = map(anguloElevacao, 0, 1023, 0, 180);
  servoElevacao.write(anguloElevacao);

  anguloGarra = analogRead(potGarra);
  anguloGarra = map(anguloGarra, 0, 1023, 0, 180);
  servoGarra.write(anguloGarra);

  servosPos();
}

void servosPos()
{
  Serial.println("--------------------------------------------------");
  Serial.print(servoBase.read());
  Serial.print(" - ");
  Serial.print(servoAvanco.read());
  Serial.print(" - ");
  Serial.print(servoElevacao.read());
  Serial.print(" - ");
  Serial.println(servoGarra.read());
  Serial.println("--------------------------------------------------");
}

//s1 = Base | 6 = Direita, 70 = Frente, 156 = Esquerda
//s2 = Garra | 10 = Aberta, 90 = Fechada
//s3 = Avanço | 177 = Esticado, 93 = Dobrado
//s4 = Elevação | 178 = Cima, 105 = Baixo
