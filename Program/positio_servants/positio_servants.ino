#include <Servo.h>

#define potBase A0
#define potGarra A1
#define potAvanco A2
#define potElevacao A3

Servo servoBase;
Servo servoGarra;
Servo servoAvanco;
Servo servoElevacao;

int anguloBase;
int anguloElevacao;
int anguloAvanco;
int anguloGarra;

int tmp = 1000;

void setup() 
{
  servoBase.attach(2);
  servoGarra.attach(3);
  servoAvanco.attach(4);
  servoElevacao.attach(5);
  
  Serial.begin(9600);
}

void loop() 
{
  anguloBase = analogRead(potBase);
  anguloBase = map(anguloBase, 0, 1023, 0, 180);
  servoBase.write(anguloBase);
  
  anguloElevacao = analogRead(potElevacao);
  anguloElevacao = map(anguloElevacao, 0, 1023, 0, 180);
  servoElevacao.write(anguloElevacao);

  anguloAvanco = analogRead(potAvanco);
  anguloAvanco = map(anguloAvanco, 0, 1023, 0, 180);
  servoAvanco.write(anguloAvanco);
  
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
  Serial.print(servoGarra.read());
  Serial.print(" - ");
  Serial.print(servoAvanco.read());
  Serial.print(" - ");
  Serial.println(servoElevacao.read());
  Serial.println("--------------------------------------------------");
}

//s1 = Base | 6 = Direita, 70 = Frente, 156 = Esquerda
//s2 = Garra | 10 = Aberta, 90 = Fechada
//s3 = Avanço | 177 = Esticado, 93 = Dobrado
//s4 = Elevação | 178 = Cima, 105 = Baixo
