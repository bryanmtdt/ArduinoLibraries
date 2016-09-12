#include <ESP8266_XYZ.h>
#include <SoftwareSerial.h>

#define TRG 4
#define SSID F("iMA6iNEXYZ")
#define PASS F("16davinci")
#define ESP_RX   3 //Intercambiar pines para que coinciadan con los ejemplos anteriores
#define ESP_TX   2 //Intercambiar pines para que coinciadan con los ejemplos anteriores

// SoftwareSerial Serail1(TX,RX)
int n = 20000;
SoftwareSerial swSerial(ESP_RX, ESP_TX); //Pines del ESP: RXD a D3, TXD a D2;

ESP8266_XYZ esp(&swSerial);

void setup() {
  Serial.begin(115400);             // Inicializacion del Monitor Serial a 9600
  swSerial.begin(9600);
  Serial.println("Init");
  while (!esp.espTest());
  bool conn_status = esp.connectAP(SSID, PASS);
  Serial.println("Setup OK");//Sino conecta no hay setup ok
  Serial.print("Connection status: ");
  Serial.println(conn_status);
}

float distancia() {
  unsigned int time;
  unsigned int sizeofpulse;
  pinMode(TRG, OUTPUT); //return digital pin to OUTPUT mode after reading
  digitalWrite(TRG, LOW);
  delayMicroseconds(25);
  digitalWrite(TRG, HIGH); //Trig pin pulsed LOW for 25usec
  time = micros(); //record timer
  pinMode(TRG, INPUT); //change pin to INPUT to read the echo pulse
  sizeofpulse = pulseIn(TRG, LOW, 18000); //should be approx 150usec, timeout at 18msec
  time = micros() - time - sizeofpulse; // amount of time elapsed since we sent the trigger pulse and detect the echo pulse, then subtract the size of the echo pulse
  return (time * 343.21 / 2 / 10000) - 7.3; // convert to distance in centimeters
}

void loop() {
  //lectura de sensor
  //float sensor_value = distancia();


  n += 1;
  esp.addToJson("Distancia", n);
  int resp = esp.httpPost("www.imaginexyz.com", "/rentacar/pos", 80);

  Serial.print("Response: ");
  Serial.println(resp);
  Serial.println();
  delay(15000);
  Serial.println("GEEEEEEET");
  int resp2 = esp.httpGet("www.imaginexyz.com", "/rentacar/last", 80, resp2);
  Serial.println(resp2);

  Serial.println("FIN       GEEEEEEET");
  delay(5000);

  
  



}
