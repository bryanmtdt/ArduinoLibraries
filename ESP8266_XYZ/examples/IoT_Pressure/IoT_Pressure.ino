#include <SFE_BMP180.h>
#include <Wire.h>
#include <ESP8266_XYZ.h>
#include <SoftwareSerial.h>

#define ALTITUDE 1200.0

#define SSID F("iMA6iNEXYZ")  //Nombre de la red a la que se desea conectar
#define PASS F("16davinci")   //Contraseña de la red
#define server F("www.imaginexyz.com")  //Servidor
#define PostPath F("/rentacar/pos")     //Ruta del Post
#define GetPath F("/rentacar/last")     //Ruta del Get
#define ESP_TX   3            //Pin de TX del ESP
#define ESP_RX   2            //Pin de RX del ESP

SoftwareSerial swSerial(ESP_TX, ESP_RX); //Pines del ESP: TXD a D3, RXD a D2; (3,2)

ESP8266_XYZ esp(&swSerial, 4);    //Arg #1: puntero al stream, Arg #2: pin para reset

SFE_BMP180 pressure;

char status;
double T, P, p0, a;

void setup()
{
  Serial.begin(9600);             //Inicializacion del Monitor Serial a 9600
  swSerial.begin(9600);           //Inicializacion de SoftwareSerial a 9600

  Serial.println("Init");         //Mensaje de inicialización

  //No se continúa hasta asegurar el buen funcionamiento del dispositivo
  while (!esp.espTest());
  while (!esp.softReset());
  while (!esp.connectAP(SSID, PASS));

  Serial.println("Setup OK");     //Si se muestra existe conexión

  Serial.println("REBOOT");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while (1); // Pause forever.
  }
}

void enviarP() {
  esp.addToJson("Sensor", "Presion+Temp_1");
  esp.addToJson("Presion", String(P));
  esp.addToJson("Temperatura", String(T));
  int resp = esp.httpPost(server, PostPath, 80);
  Serial.println(resp);
}

void recibirP() {
  String str_resp;
  int resp = esp.httpGet(server, GetPath, 80, &str_resp);
  Serial.print(resp);
  Serial.print(" ");
  Serial.println(str_resp);

  String valor;
  esp.getJsonAttribute(str_resp, "Presion", &valor);//(JSON completo, atributo buscado, valor)
  Serial.println(valor);
  esp.getJsonAttribute(str_resp, "Temperatura", &valor);//(JSON completo, atributo buscado, valor)
  Serial.println(valor);
}

void Presion_Temp() {
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      Serial.print("temperature: ");
      Serial.println(T, 2);
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          Serial.println(" mb, ");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  //enviarP();
}

void loop()
{
  Presion_Temp();

}
