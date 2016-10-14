#include <ESP8266_XYZ_StandAlone.h>

#define DEBUG

const char* SSID = "iMA6iNEXYZ";  //Nombre de la red a la que se desea conectar
const char* PASS = "16davinci";  //Contraseña de la red
//const char* SSID = "Consultur_EXT";  //Nombre de la red a la que se desea conectar
//const char* PASS = "turismo07";   //Contraseña de la red
const char* server = "iot-xyz.herokuapp.com";  //Servidor
const int http_port = 80;
const char* post_path = "/sensor/";     //Ruta del Post 
const char* get_path = "/last/";     //Ruta del Get

ESP8266_XYZ esp;

#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 13); // (Trig PIN,Echo PIN)

int distancia;

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Connecting ");
#endif

  while(!esp.connectAP(SSID, PASS)){
    delay(200);
  }

#ifdef DEBUG
  Serial.println("WiFi connected");
#endif


  pinMode(5, OUTPUT);
}

void enviarU() {
  String resp;
  esp.addToJson("Sensor", "Ultrasonico_4");
  esp.addToJson("Valor", String(distancia));
  int code = esp.httpPost(server, post_path, http_port, &resp);

#ifdef DEBUG
  Serial.print("Response code: ");
  Serial.println(code);
  Serial.print("Response body: ");
  Serial.println(resp);
  
#endif
}

void recibirU() {
 String resp;
 int code = esp.httpGet(server, get_path, http_port, &resp);

#ifdef DEBUG
  Serial.print("Response code: ");
  Serial.println(code);
  Serial.print("Response body: ");
  Serial.println(resp);
#endif

}

void ultrasonico() {
  distancia = ultrasonic.Ranging(CM);

#ifdef DEBUG
  Serial.println(distancia);
#endif

}

void loop() {
  ultrasonico();
  enviarU();
  //recibirU();
  delay(1000);
}
