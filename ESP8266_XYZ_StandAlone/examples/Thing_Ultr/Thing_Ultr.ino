#include <ESP8266_XYZ.h>
#include <ESP8266WiFi.h>
#define DEBUG

//const char* SSID = "iMA6iNEXYZ";  //Nombre de la red a la que se desea conectar
//const char* PASS = "16davinci";   //Contraseña de la red
const char* SSID = "Consultur_EXT";  //Nombre de la red a la que se desea conectar
const char* PASS = "turismo07";   //Contraseña de la red
const char* server = "https://iot-xyz.herokuapp.com";  //Servidor

//https://iot-xyz.herokuapp.com/all

#define server2  F("https://iot-xyz.herokuapp.com") // Servidor
#define PostPath F("/sensor/")     //Ruta del Post 
#define GetPath F("/sensor/")     //Ruta del Get

const int httpPort = 3000;
String response;

ESP8266_XYZ esp;

WiFiClient client;

#include <Ultrasonic.h>
#include "Notes.h"

#define melodyPin 4

Ultrasonic ultrasonic(12, 13); // (Trig PIN,Echo PIN)

int distancia;
int rangoinferior = 15;
int rangosuperior = 20;

/////////////////////////////////////find and keep or delete

long delayValue = 0;
long numCycles = 0;
int caso = 1;
int thisNote = 0;
long counterXdelay = 0;

int pauseBetweenNotes = 0;
int noteDuration = 0;

long counterXdelaymicros = 0;

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

int size = sizeof(melody) / sizeof(int);

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

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Connecting ");
#endif

  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

#ifdef DEBUG
    Serial.print("-");
#endif

  }

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif


  pinMode(5, OUTPUT);
  pinMode(melodyPin, OUTPUT);
  //analogWriteRange(10);
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
  digitalWrite(5, HIGH);
  delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    //delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    //delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(5, LOW);
}


int readResponse() {

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 500) {

#ifdef DEBUG
      Serial.println(">>> Client Timeout !");
#endif

      client.stop();
      return 0;
    }
  }

  //Banderas de control del parseo
  bool line_blank = true;
  bool http_body = false;
  bool in_status = false;

  //Variables de soporte
  char status_code[4];
  int i = 0;
  int code = 0;

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {

    char c = client.read();

    //Luego del primer espacio se encuentra el código de estado
    if (c == ' ' && !in_status) {
      in_status = true;
    }

    //Se lee el código de estado de 3 dígitos
    if (in_status && i < 3 && c != ' ') { ////Qué pasa si el código es de 4 digitos???
      status_code[i] = c;
      i++;
    }

    //Se parsea el código de char* a int
    if (i == 3) {
      status_code[i] = '\0';
      code = atoi(status_code);
    }

    if (http_body) {
      //Si se detecta el cuerpo de la respuesta
      //se escribe en response (si existe)
      response.concat(c);
    }
    else {
      //Condiciones para detectar el cuerpo de la respuesta
      if (c == '\n' && line_blank) {
        http_body = true;
      }

      if (c == '\n') {
        line_blank = true;
      }
      else if (c != '\r') {
        line_blank = false;
      }
    }
  }

  response.remove(0, response.indexOf('{'));
  response.remove(response.indexOf('}') + 1);
  return code;
}


void enviarU() {
  client.stop();
  if (!client.connect(server, httpPort)) {

#ifdef DEBUG
    Serial.println("connection failed");
#endif

    return;
  }
  String Header_Msg;
  esp.addToJson("Sensor", "Ultrasonico_4");
  esp.addToJson("Valor", String(distancia));
  Header_Msg = esp.httpPostHeaderMsg(server2, PostPath, httpPort, Header_Msg);
  client.print(Header_Msg);
  int code;
  code = readResponse();

#ifdef DEBUG
  Serial.println(code);
  //Serial.println(response);
#endif

  if (code == 201) {
    response = "";
    //recibirU();
  }

  response = "";
}

void recibirU() {
  client.stop();
  if (!client.connect(server, httpPort)) {

#ifdef DEBUG
    Serial.println("connection failed");
#endif

    return;
  }
  String Header_Msg;
  Header_Msg = esp.httpGetHeaderMsg(server2, GetPath, httpPort, Header_Msg);
  client.print(Header_Msg);
  int code;
  code = readResponse();

#ifdef DEBUG
  Serial.println(code);
  Serial.println(response);
#endif

  response = "";
}

void ultrasonico() {
  distancia = ultrasonic.Ranging(CM);

#ifdef DEBUG
  Serial.println(distancia);
#endif

  if ((distancia >= rangoinferior) && (rangosuperior >= distancia)) {
    analogWrite(melodyPin, 30);
  }

  else {
    analogWrite(melodyPin, 300);
  }
}

void loop() {
  ultrasonico();
  enviarU();
  delay(50);
}
