#include <EEPROM.h>
#include <SoftwareSerial.h> // Chama a Biblioteca para bluetooth, quando quero usar outras portas
#include <VarSpeedServo.h>

//Declaração dos pinos do potenciômetro
#define potBase     A0
#define potAvanco   A1
#define potElevacao A2
#define potGarra    A3

//Declaração dos pinos do Joystick
#define potBaseVrx     A7  // Eixo horizontal - Base
#define potAvancoVry   A6  // Eixo vertical - Avanço
#define potGarraVrx    A5  // Eixo horizontal - Garra
#define potElevacaoVry A4  // Eixo vertical - Elevação

//Declaração dos pinos dos botões
#define pinBotaoReset  6  // Botão Vermelho: Reset
#define pinBotaoGravar 7  // Botão Verde: Gravar
#define pinBotaoAuto   8  // Botão Azul: Automático
#define pinBotaoManual 9  // Botão Amarelo: Manual

//Declaração dos pinos dos servos
#define pinServoBase     2
#define pinServoAvanco   3
#define pinServoElevacao 4
#define pinServoGarra    5

#define pausa 90 // Pausa referente ao VarSpeedServo
#define LED 12

int bluetoothTx = 10; // Transmissão do Bluetooth no pino 10, liga no pino 10 do arduino
int bluetoothRx = 11; // Recepção do Bluetooth no pino 11, liga no pino 11 do Arduino

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
VarSpeedServo servoBase, servoAvanco, servoElevacao, servoGarra;

int anguloBase, anguloAvanco, anguloElevacao, anguloGarra;
bool modoPot = false, modoJoystick = false, modoBluetooth = false, modoAutomatico = false;
int x = 0;
byte add = 0;
int opcaoControle = 0;

void setup()
{
  //Setup da conexão serial USB com o computador
  Serial.begin(9600);
  //Setup da conexão serial BLUETOOTH com o android
  bluetooth.begin(9600);

  servoBase.attach(pinServoBase);         // Anexar o motor 01 ao pino 02 (GIRO)
  servoAvanco.attach(pinServoAvanco);     // Anexar o motor 02 ao pino 03 (AVANÇO)
  servoElevacao.attach(pinServoElevacao); // Anexar o motor 03 ao pino 04 (ELEVAÇÃO)
  servoGarra.attach(pinServoGarra);       // Anexar o motor 04 ao pino 05 (GARRA)

  // Declarando os botões como INPUT_PULLUP para usar os resistores internos do Arduino
  pinMode(pinBotaoGravar, INPUT_PULLUP);
  pinMode(pinBotaoAuto, INPUT_PULLUP);
  pinMode(pinBotaoManual, INPUT_PULLUP);
  pinMode(pinBotaoReset, INPUT_PULLUP);
}

void loop()
{
  if (!digitalRead(pinBotaoGravar)) {  //Caso o botão gravar seja pressionado, chama a função para gravar as posições
    //Serial.println("Tô no modo gravação!");
    delay(250);
    gravar(); // Função responsável por gravar as posições do braço
  }
  else if (!digitalRead(pinBotaoAuto)) { // Caso o botão automático seja pressionado, chama a função automático e inicia o ciclo de posições gravadas
    //Serial.println("Tô no modo automático!");
    delay(250);
    modoAutomatico = true; // Ativa o ciclo de trabalho, enquanto for verdadeiro continua a execução do modo automático
    automatico();          // Função responsável por deixar o ciclo de gravações no modo automático
  }
  else if (!digitalRead(pinBotaoManual)) {
    // Faz a seleção do tipo de controle que desejo utilizar, por exemplo, potenciômetro, Joystick, Bluetooth...
    if (opcaoControle < 5) { // De 0 até 4
      // Altera a quantidade de controladores aqui
      opcaoControle++;
    } else {
      opcaoControle = 0;
    }

    if (opcaoControle == 1) {
      //Serial.println("Selecionado - Potenciômetro");
      modoPot = true; // Ativa o ciclo de trabalho, enquanto for verdadeiro continua a execução do modo potenciômetro
      manualPot();    // Função responsável por deixar fazer o controle manual dos potenciômetros
    }
    else if (opcaoControle == 2) {
      //Serial.println("Selecionado - Joystick");
      modoJoystick = true; // Ativa o ciclo de trabalho, enquanto for verdadeiro continua a execução do modo Joystick
      manualJoystick();    // Função responsável por deixar fazer o controle manual dos Joysticks
    }
    else if (opcaoControle == 3) {
      //Serial.println("Selecionado - Bluetooth");
      modoBluetooth = true; // Ativa o ciclo de trabalho, enquanto for verdadeiro continua a execução do App bluetooth
      manualBluetooth();    // Função responsável por deixar fazer o controle manual através do celular via bluetooth
      opcaoControle = 0;    // Retorna para zero, iniciando a contagem
    }
    while (!digitalRead(pinBotaoManual)) {
      // Enquanto o botão manual estiver pressionado, não faz nada
    }
    delay(250);
  }
}

void gravar()
{
  //Serial.println("Gravação!");
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

void automatico()
{
  //Serial.println("Automático");
  byte ADD = 0;
  while (modoAutomatico)
  {
    for (x = 0; x < 64; x++)
    {
      // Faz a leitura das posições que foram pré-gravadas na memória
      ADD = ADD + 1;
      anguloBase = EEPROM.read(ADD);
      ADD = ADD + 1;
      anguloGarra = EEPROM.read(ADD);
      ADD = ADD + 1;
      anguloAvanco = EEPROM.read(ADD);
      ADD = ADD + 1;
      anguloElevacao = EEPROM.read(ADD);

      if (!digitalRead(pinBotaoReset)) { // Caso queira interromper o modo automático, basta apertar o botão reset por alguns segundos
        //Serial.println("Resetando...");
        modoAutomatico = false;
      }

      if (anguloBase == 0) break;
      else {
        // Posiciona os servos no ângulo inicial
        servoBase.write(anguloBase, pausa);
        servoAvanco.write(anguloAvanco, pausa);
        servoElevacao.write(anguloElevacao, pausa);
        servoGarra.write(anguloGarra, pausa);
      }
      delay(1000);
    }
    ADD = 0;
  }
}

void manualPot()
{
  //Serial.println("Manual");
  while (modoPot)
  {
    readPot();
    anguloServos();

    if (!digitalRead(pinBotaoReset)) {
      //Serial.println("Resetando...");
      delay(250);
      modoPot = false;
      statusLED();
    }
    else if (!digitalRead(pinBotaoGravar)) {
      delay(250);
      gravar();
    }
    delay(1);
  }
}

void manualJoystick() {
  while (modoJoystick)
  {
    readJoystick();
    anguloServos();

    if (!digitalRead(pinBotaoReset)) {
      //Serial.println("Resetando...");
      delay(250);
      modoJoystick = false;
      statusLED();
    }
    else if (!digitalRead(pinBotaoGravar)) {
      delay(250);
      gravar();
    }
    delay(1);
  }
}

void manualBluetooth() {
  while (modoBluetooth)
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

    if (!digitalRead(pinBotaoReset)) {
      //Serial.println("Resetando...");
      delay(250);
      modoBluetooth = false;
      statusLED();
    }
    else if (!digitalRead(pinBotaoGravar)) {
      delay(250);
      gravar();
    }
    delay(1);
  }
}

void readPot() {
  anguloBase = analogRead(potBase);
  anguloAvanco = analogRead(potAvanco);
  anguloElevacao = analogRead(potElevacao);
  anguloGarra = analogRead(potGarra);
}

void readJoystick() {
  anguloBase = analogRead(potBaseVrx);
  anguloAvanco = analogRead(potAvancoVry);
  anguloElevacao = analogRead(potElevacaoVry);
  anguloGarra = analogRead(potGarraVrx);
}

void statusLED() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

void anguloServos() {
  anguloBase = map(anguloBase, 0, 1023, 0, 177);
  servoBase.write(anguloBase, pausa);

  anguloAvanco = map(anguloAvanco, 0, 1023, 40, 140);
  servoAvanco.write(anguloAvanco, pausa);

  anguloElevacao = map(anguloElevacao, 0, 1023, 0, 130);
  servoElevacao.write(anguloElevacao, pausa);

  anguloGarra = map(anguloGarra, 0, 1023, 10, 90);
  servoGarra.write(anguloGarra, pausa);
}
