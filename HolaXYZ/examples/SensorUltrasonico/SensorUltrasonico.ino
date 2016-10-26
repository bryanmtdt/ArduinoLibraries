//Taller de Arduino
//Uso del sensor ultrasónico

// LLamado de Biblioteca

#include <Ultrasonic.h>

//Definición de variable sensor

Ultrasonic sensor(9,8);   // (Trig PIN,Echo PIN)


void setup() {

  //Inicialización del Puerto Serial
  Serial.begin(9600); 
}

void loop() {

  //Imprimir valores
  Serial.print("Distancia: ");
  Serial.print(sensor.Ranging(CM)); // Usar CM para centimetros y IN para pulgadas
  Serial.print(" cm     " );
}
