#include <ESP8266_XYZ.h>
#include <ESP8266WiFi.h>
#define DEBUG

//const char* SSID = "IoT-XYZ";  //Nombre de la red a la que se desea conectar
//const char* PASS = "imagineRocks";   //Contraseña de la red
//const char* server = "192.168.1.101";  //Servidor
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

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire ourWire(2); //Se establece el pin declarado como bus para la comunicación OneWire

DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

float temp;

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
#endif

  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

#ifdef DEBUG
    Serial.print(".");
#endif

  }

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  sensors.begin(); //Se inician los sensores
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


void enviarP() {
  client.stop();
  if (!client.connect(server, httpPort)) {

#ifdef DEBUG
    Serial.println("connection failed");
#endif

    return;
  }
  String Header_Msg;
  esp.addToJson("Sensor", "Presion+Temp_1");
  esp.addToJson("Valor", String(temp));
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
    //recibirP();
  }

  response = "";
}

void recibirP() {
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

void Temp() {
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
  temp = sensors.getTempCByIndex(0);
#ifdef DEBUG
  Serial.print(temp);
#endif
}


void loop() {
  Temp();
  enviarP();
  //delay(50);
}
