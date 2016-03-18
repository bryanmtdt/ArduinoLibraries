/*iMA6iNEXYZ
//Curso Explorando el universo robótico
Ejemplo Motores

Este ejemplo servirá como ejemplo para la demostración de: 
  % Declaración de variables
  % Sección de configuración en el código
  % Declaración e Implementación de Funciones
  %Uso de pines digitales
  %Utilizacin de motores
*/

//Inicio de programa

// Declaración de variables
//Pines 5 (velocidad)  y 10 (direccion) corresponden al motor izquierdo
//Pines 6 (velocidad) y 11 (direccion) corresponden al motor derecho
// Se debe conectar la alimentacion externa

const int izqvel=5;
const int izqdir=10;
const int dervel=6;
const int derdir=11;

void setup() {
  pinMode(izqvel,OUTPUT);
  pinMode(izqdir,OUTPUT);
  pinMode(dervel,OUTPUT); 
  pinMode(derdir,OUTPUT);

  

}

void loop() {
  adelante();
}


// Funcion para mover motores hacia adelante

void adelante(){
  analogWrite(dervel,120);
  analogWrite(izqvel,120);
  digitalWrite(derdir, HIGH);
  digitalWrite(izqdir,HIGH);
  delay(1000);
}

