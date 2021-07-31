#include <SoftwareSerial.h> // Chama a Biblioteca para bluetooth, quando quero usar outras portas
#include <Servo.h>          // Chama a biblioteca do Servo

//Declaração dos pinos do servo
#define pinServoBase     2
#define pinServoAvanco   3
#define pinServoElevacao 4
#define pinServoGarra    5

int bluetoothTx = 10; // Transmissão do Bluetooth no pino 10, liga no pino 10 do arduino
int bluetoothRx = 11; // Recepção do Bluetooth no pino 11, liga no pino 11 do Arduino

int anguloBase, anguloAvanco, anguloElevacao, anguloGarra;

Servo servoBase, servoAvanco, servoElevacao, servoGarra;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  servoBase.attach(pinServoBase);         // Anexar o motor 01 ao pino 02 (GIRO)
  servoAvanco.attach(pinServoAvanco);     // Anexar o motor 02 ao pino 03 (AVANÇO)
  servoElevacao.attach(pinServoElevacao); // Anexar o motor 03 ao pino 04 (ELEVAÇÃO)
  servoGarra.attach(pinServoGarra);       // Anexar o motor 04 ao pino 05 (GARRA)

  //Setup da conexão serial USB com o computador
  Serial.begin(9600);

  //Setup da conexão serial BLUETOOTH com o android
  bluetooth.begin(9600);

}
void loop()
//Lê o Bluetooth e escreve na porta serial USB
{
  if (bluetooth.available() >= 2)
  {
    unsigned int servopos = bluetooth.read();
    unsigned int servopos1 = bluetooth.read();
    unsigned int realservo = (servopos1 * 256) + servopos;
    //Serial.println(realservo);

    if (realservo >= 1000 && realservo < 1180) { // Posição real do servo
      int anguloBase = realservo;
      anguloBase = map(anguloBase, 1000, 1180, 0, 180);
      servoBase.write(anguloBase);
      //Serial.println(anguloBase);
      delay(10);
    }

    else if (realservo >= 2000 && realservo < 2180) {
      int anguloAvanco = realservo;
      anguloAvanco = map(anguloAvanco, 2000, 2180, 0, 180);
      servoAvanco.write(anguloAvanco);
      //Serial.println("servo 2 On");
      delay(10);
    }

    else if (realservo >= 3000 && realservo < 3180) {
      int anguloElevacao = realservo;
      anguloElevacao = map(anguloElevacao, 3000, 3180, 0, 180);
      servoElevacao.write(anguloElevacao);
      //Serial.println("servo 3 On");
      delay(10);
    }

    else if (realservo >= 4000 && realservo < 4180) {
      int anguloGarra = realservo;
      anguloGarra = map(anguloGarra, 4000, 4180, 0, 180);
      servoGarra.write(anguloGarra);
      //Serial.println("servo 4 On");
      delay(10);
    }
  }
}
