#include <EEPROM.h>
#include <VarSpeedServo.h>

//Declaração dos pinos do potenciômetro
#define potBase     A0
#define potAvanco   A1
#define potElevacao A2
#define potGarra    A3
//Declaração dos pinos do Joystick
#define potBaseVrx     A7
#define potAvancoVry   A6
#define potGarraVrx    A5
#define potElevacaoVry A4

#define pinBotaoReset  6 // Botão Vermelho: Reset
#define pinBotaoGravar 7 // Botão Verde: Gravar
#define pinBotaoAuto   8 // Botão Azul: Automático
#define pinBotaoManual 9 // Botão Amarelo: Manual
#define pinBotaoJoystick 11

//Declaração dos pinos do servo
#define pinServoBase     2
#define pinServoAvanco   3
#define pinServoElevacao 4
#define pinServoGarra    5

VarSpeedServo servoBase, servoAvanco, servoElevacao, servoGarra;
#define pausa 120

int anguloBase, anguloAvanco, anguloElevacao, anguloGarra;
bool modoManual = false, modoJoystick = false, modoAutomatico = false;
int x = 0;
byte add = 0;

void setup()
{
  //Serial.begin(9600);

  servoBase.attach(pinServoBase);         // Anexar o motor 01 ao pino 02 (GIRO)
  servoAvanco.attach(pinServoAvanco);     // Anexar o motor 02 ao pino 03 (AVANÇO)
  servoElevacao.attach(pinServoElevacao); // Anexar o motor 03 ao pino 04 (ELEVAÇÃO)
  servoGarra.attach(pinServoGarra);       // Anexar o motor 04 ao pino 05 (GARRA)

  pinMode(pinBotaoGravar, INPUT_PULLUP);
  pinMode(pinBotaoAuto, INPUT_PULLUP);
  pinMode(pinBotaoManual, INPUT_PULLUP);
  pinMode(pinBotaoJoystick, INPUT_PULLUP);
  pinMode(pinBotaoReset, INPUT_PULLUP);
}

void loop()
{
  if (!digitalRead(pinBotaoGravar)) {  //Caso o botão gravar for pressionado, chama a função para gravar as posições
    //Serial.println("Tô no modo gravação!");
    delay(250);
    gravar();
  }
  else if (!digitalRead(pinBotaoAuto)) { // Caso o botão automático for pressionado, chama a função automático e inicia o ciclo de posições gravadas
    //Serial.println("Tô no modo automático!");
    delay(250);
    modoAutomatico = true;
    automatico();
  }
  else if (!digitalRead(pinBotaoManual)) { // Caso o botão manual seja pressionado, é possível controlar o braço pelos potenciômetros
    delay(250);
    modoManual = true;
    manual();
  }
  else if (!digitalRead(pinBotaoJoystick)) { // Caso o botão manual seja pressionado, é possível controlar o braço pelos potenciômetros
    delay(250);
    modoJoystick = true;
    manualJoystick();
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

void automatico() {
  //Serial.println("Automático");
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

      if (!digitalRead(pinBotaoReset)) { // Caso queira interromper o modo automático, basta apertar o botão reset
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

void manual()
{
  //Serial.println("Manual");
  while (modoManual)
  {
    readPot();
    anguloServos();

    if (!digitalRead(pinBotaoReset)) {
      //Serial.println("Resetando...");
      delay(250);
      modoManual = false;
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
