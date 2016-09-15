//Bibliotecas que se deben incluir
#include <ESP8266_XYZ.h>
#include <SoftwareSerial.h>

#define SSID F("iMA6iNEXYZ")  //Nombre de la red a la que se desea conectar
#define PASS F("16davinci")   //Contraseña de la red
#define server F("www.imaginexyz.com")  //Servidor
#define PostPath F("/rentacar/pos")     //Ruta del Post
#define ESP_RX   3            //Pin de RX del ESP
#define ESP_TX   2            //Pin de TX del ESP

int n = 20000;
int m = 20000;
SoftwareSerial swSerial(ESP_RX, ESP_TX); //Pines del ESP: RXD a D3, TXD a D2;

ESP8266_XYZ esp(&swSerial, 4);    //Arg #1: puntero al stream, Arg #2: pin para reset

void setup() {
  Serial.begin(9600);             //Inicializacion del Monitor Serial a 9600
  swSerial.begin(9600);           //Inicializacion de SoftwareSerial a 9600
  Serial.println("Init");         //Mensaje de inicialización
  
  //No se continúa hasta asegurar el buen funcionamiento del dispositivo
  while (!esp.espTest());         
  while (!esp.softReset());
  while (!esp.connectAP(SSID, PASS));
  
  Serial.println("Setup OK");     //Si se muestra existe conexión
  
}

void loop() {

  //Se agrega el contador n al JSON con nombre "ContadorN"
  esp.addToJson("ContadorN", n);
  
  //Se agrega el contador m al JSON con nombre "ContadorM"
  esp.addToJson("ContadorM", m);

  //Se envía la solicitud POST al servidor 
  int resp = esp.httpPost(server, PostPath, 80);
  
  //Se imprime el código de respuesta del servidor
  Serial.print("Respuesta: ");
  Serial.println(resp);
  Serial.println();

  //Se incrementa contador de ejemplo 
  n++;
  //Se decrementa el contador de ejemplo
  m--;
  
  delay(1000);         //Suspende operaciones por 1 segundos
}
