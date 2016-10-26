// Taller Arduino Básico
//ImagineXYZ

//Ejemplo sensor de temperatura

//Llamado de bibliotecas
#include "DHT.h"

#define DHTPIN 2     //Declaración del pin digital a donde se va a conectar

//Definición del tipo de sensor a utilizar
//Descomentar el sensor que se usara

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//Conectar el pin 1 (izquierda) a +5V
// NOTE:si se usa una placa de 3.3V, se debe conectar a 3V en lugar de 5V
// Conectar el pin 2 (centro) al número asignado en DHTPIN
// Conectar el pin 2 (izquierda) a GND

// Inicialización del sensor
// Arranque de la secuencia de control para el dispositivo
DHT dht(DHTPIN, DHTTYPE);

//Inicialización de configuración
void setup() {
  //Abrir puerto serial
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  //Inicialización de ssensor para adquisición de datos
  dht.begin();
}

void loop() {
  //Se debe esperar un tiempo para la adquisición del sensor
  delay(2000);

  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); //Lectura temperatura en Celsius 
  float f = dht.readTemperature(true); //Lectura de la temperatura en Farenheit

//Verificación de las lecturas al sensor
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

// Impresión de los valores obtenidos

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
}
