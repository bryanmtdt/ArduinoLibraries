#include "Notes.h"
#include <Ultrasonic.h>
#include <ESP8266_XYZ.h>
#include <SoftwareSerial.h>

#define melodyPin 5

#define SSID F("iMA6iNEXYZ")  //Nombre de la red a la que se desea conectar
#define PASS F("16davinci")   //Contraseña de la red
#define server F("www.imaginexyz.com")  //Servidor
#define PostPath F("/rentacar/pos")     //Ruta del Post
#define GetPath F("/rentacar/last")     //Ruta del Get
#define ESP_TX   3            //Pin de TX del ESP
#define ESP_RX   2            //Pin de RX del ESP

Ultrasonic ultrasonic(9, 8); // (Trig PIN,Echo PIN)
SoftwareSerial swSerial(ESP_TX, ESP_RX); //Pines del ESP: TXD a D3, RXD a D2; (3,2)

ESP8266_XYZ esp(&swSerial, 4);    //Arg #1: puntero al stream, Arg #2: pin para reset

int distancia;
int rangoinferior = 15;
int rangosuperior = 20;

int melody[] = {
  NOTE_E4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4, 0, NOTE_A4, NOTE_G4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4, 0, NOTE_A4, NOTE_G4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4, 0, NOTE_A4, NOTE_E4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4,
  //38

  NOTE_E4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4, 0, NOTE_A4, NOTE_G4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4, 0, NOTE_A4, NOTE_G4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4, 0, NOTE_A4, NOTE_G4,
  0, 0, 0, NOTE_E4, 0, NOTE_G4, NOTE_A4,

  0, 0,
};

//Mario main them tempo
int tempo[] = {
  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,

  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,

  10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,

  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,

  10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,
  10, 10, 10, 10,
};

int size = sizeof(melody) / sizeof(int);

long delayValue = 0;
long numCycles = 0;
int caso = 1;
int thisNote = 0;
long counterXdelay = 0;

int pauseBetweenNotes = 0;
int noteDuration = 0;

long counterXdelaymicros = 0;

void setup() {
  Serial.begin(9600);             //Inicializacion del Monitor Serial a 9600
  swSerial.begin(9600);           //Inicializacion de SoftwareSerial a 9600

  Serial.println("Init");         //Mensaje de inicialización

  //No se continúa hasta asegurar el buen funcionamiento del dispositivo
  while (!esp.espTest());
  while (!esp.softReset());
  while (!esp.connectAP(SSID, PASS));

  Serial.println("Setup OK");     //Si se muestra existe conexión

  pinMode(melodyPin, OUTPUT);
  analogWrite(melodyPin, 0);
}


void play() {
  if ((millis() - counterXdelay) >= pauseBetweenNotes) { // to distinguish the notes, set a minimum time between them the note's duration + 30% seems to work well
    noteDuration = 1000 / tempo[thisNote]; // to calculate the note duration, take one second divided by the note type.e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    pauseBetweenNotes = noteDuration * 1.0;
    counterXdelay = millis();
    buzz(melodyPin, melody[thisNote], noteDuration);
    thisNote++;
  }
  if (thisNote > (size - 1)) {
    thisNote = 0;
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
}

void enviarU() {
  esp.addToJson("Sensor", "Ultrasonico_1");
  esp.addToJson("Valor", distancia);
  int resp = esp.httpPost(server, PostPath, 80);
  Serial.println(resp);
}

void recibirU() {
  String str_resp;
  int resp = esp.httpGet(server, GetPath, 80, &str_resp);
  Serial.print(resp);
  Serial.print(" ");
  Serial.println(str_resp);

  String valor;
  esp.getJsonAttribute(str_resp, "Valor", &valor);//(JSON completo, atributo buscado, valor)
  Serial.println(valor);
}

void ultrasonico() {
  distancia = ultrasonic.Ranging(CM);
  Serial.println(distancia);

  if ((distancia >= rangoinferior) && (rangosuperior >= distancia)) {
    analogWrite(melodyPin, 0);
  }

  else {
    play();
  }
  //enviarU();
}



void loop() {
  ultrasonico();

}



/*

  //Mario main theme melody
  int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
  };*/







/*void play() {

  Serial.println(caso);

  switch (caso) {
  case 1: //buzz(melodyPin, melody[thisNote], noteDuration);
    if ((millis() - counterXdelay) >= pauseBetweenNotes) { // to distinguish the notes, set a minimum time between them the note's duration + 30% seems to work well
      noteDuration = 1000 / tempo[thisNote]; // to calculate the note duration, take one second divided by the note type.e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      pauseBetweenNotes = noteDuration * 1.0;
      counterXdelay = millis();
      delayValue = 1000000 / melody[thisNote] / 2;
      numCycles = melody[thisNote] * noteDuration / 1000;
      counterXdelaymicros = micros();
    }
    caso = 2;
    break;

  case 2:

    //Serial.println(micros() - counterXdelaymicros);
    //Serial.println(delayValue);

    //delay(10000);
    if ((micros() - counterXdelaymicros) <= delayValue) {
      digitalWrite(melodyPin, HIGH); // write the buzzer pin high to push out the diaphram
    }
    else {
      caso = 3;
      counterXdelaymicros = micros();
    }
    break;

  case 3:
    if ((micros() - counterXdelaymicros) <= delayValue) {
      digitalWrite(melodyPin, LOW); // write the buzzer pin high to push out the diaphram
    }
    else {
      caso = 4;
      counterXdelaymicros = 0;
    }
    break;

  case 4:

    thisNote++;
    if (thisNote > (size - 1)) {
      thisNote = 0;
    }
    caso = 1;
    break;

  default:
    caso = 1;
    break;
  }
  }*/

