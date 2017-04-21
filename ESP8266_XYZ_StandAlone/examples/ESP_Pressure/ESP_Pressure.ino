#include <ESP8266_XYZ_StandAlone.h>

#define DEBUG

const char* SSID = "IoT-XYZ";  //Nombre de la red a la que se desea conectar
const char* PASS = "imagineRocks";  //Contraseña de la red
//const char* SSID = "Consultur_EXT";  //Nombre de la red a la que se desea conectar
//const char* PASS = "turismo07";   //Contraseña de la red
const char* server = "iot-xyz.herokuapp.com";  //Servidor
const int http_port = 80;
const char* post_path = "/sensor/";     //Ruta del Post 
const char* get_path = "/last/";     //Ruta del Get

ESP8266_XYZ esp;

WiFiClient client;

#include <SFE_BMP180.h>
#include <Wire.h>

#define ALTITUDE F(1200.0)

SFE_BMP180 pressure;

char status;
double T, P, p0, a;

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

  Wire.begin(0, 2); //Wire.begin(int sda, int scl)

  if (pressure.begin()) {
#ifdef DEBUG
    Serial.println("BMP180 init success");
#endif
  }


  else
  {

#ifdef DEBUG
    Serial.println("BMP180 init fail\n\n");
#endif

    while (1);
  }

}


void enviarP() {
   String resp;
   int code = esp.httpPost(server, post_path, http_port, &resp);

  #ifdef DEBUG
    Serial.print("Response code: ");
    Serial.println(code);
    Serial.print("Response body: ");
    Serial.println(resp);
  #endif
}

void recibirP() {
   String resp;
 int code = esp.httpGet(server, get_path, http_port, &resp);

  #ifdef DEBUG
    Serial.print("Response code: ");
    Serial.println(code);
    Serial.print("Response body: ");
    Serial.println(resp);
  #endif

}


void Presion_Temp() {
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {

#ifdef DEBUG
      Serial.print("temperature: ");
      Serial.println(T, 2);
#endif

      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {

#ifdef DEBUG
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          Serial.println(" mb, ");
#endif

        }
        else {
          //Serial.println("error retrieving pressure measurement\n");
        }
      }
      else {
        //Serial.println("error starting pressure measurement\n");
      }
    }
    else {
      //Serial.println("error retrieving temperature measurement\n");
    }
  }
  else {
    //Serial.println("error starting temperature measurement\n");
  }
}

void loop()
{
  Presion_Temp();
  enviarP();
  delay(50);
}
