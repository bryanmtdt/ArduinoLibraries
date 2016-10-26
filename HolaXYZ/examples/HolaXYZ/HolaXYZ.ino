//______________________Biblioteca_________________________________________
//Parte I: NA
//Parte II: NA
//Parte III:
#include <Ultrasonic.h>

//___________________Creación de objetos___________________________________
//Parte I: NA
//Parte II: NA
//Parte III:
Ultrasonic ultrasonic(9, 8); // (Trig PIN,Echo PIN)

//______________________Variables globales_________________________________

//Parte I
int led = 13;

//Parte III - parte A
int potenciometro = A0;
int valorPot = 0; //valor numerico del potenciometro en la entrada analógica

//Parte III - parte B
int fade = 0;
int led2 = 3; //PWM



void setup() {
  //Parte I
  pinMode(led, OUTPUT);

  //Parte II - parte A
  Serial.begin(9600);
  //Parte II - parte B
  //Serial.begin(115200);
}

void HolaXYZ() {
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}

void Serialmonitor() {
  int i = 0;//Contador como variable local
  for (i = 0; i <= 20; i++) {
    Serial.print("Conteo i: ");
    Serial.println(i);
    delay(200);
  }
}

void Serialplotter() {
  bool Condicion = true;
  int i = 0;
  for (i = 0; i <= 20; i++) {
    Serial.println(i);
  }
  //en este punto i vale 21
  if (i == 21) {
    while (Condicion) {
      i--;
      if (i == 0) {
        Condicion = false;
        Serial.println(i);
      }
      else {
        Serial.println(i);
      }
    }
  }
}

void desvanecer() {

  //ADC: Parte 1: Ver como varía el potenciometro a nivel de datos (Monitor Serial) y grafica (Graficador Serial)
  valorPot = analogRead(potenciometro);
  Serial.println(valorPot);

  //PWM: Parte 2: Asociar lo anterior a un LED
  fade = map(valorPot, 0, 1023, 0, 255);
  analogWrite(led2, fade); //Recordar que este comando no tiene nada que ver con los pines analógicos
}

void ultrasonico() {
  Serial.print(ultrasonic.Ranging(CM)); // CM or INC
  Serial.println(" cm");
  delay(100);

  //Serial.println(ultrasonic.Ranging(CM)); // CM or INC
}

void loop() {
  //Parte I
  HolaXYZ();   //El hola mundo en electrónica es poder encender un LED, Led que enciende y apaga cada segundo

  //Parte II
  //Parte A
  Serialmonitor();
  //parte B
  Serialplotter();  //En este ejercicio se ponen a prueba los condicionales for,if,else,while junto con el serial plotter

  //Parte III
  desvanecer();

  //Parte IV
  ultrasonico();

}
