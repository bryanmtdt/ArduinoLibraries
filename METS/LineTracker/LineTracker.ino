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
const int sensorder=7;
const int sensorizq=4;
const int sensorcen=12;

void setup() {
  pinMode(izqvel,OUTPUT);
  pinMode(izqdir,OUTPUT);
  pinMode(dervel,OUTPUT); 
  pinMode(derdir,OUTPUT);
  pinMode(sensorder,INPUT);
  pinMode(sensorizq,INPUT);
  pinMode(sensorcen,INPUT);
}

void loop() {
  seguidor();
}


// Funcion que define la velocidad
void velocidad(){
  analogWrite(dervel,40);
  analogWrite(izqvel,40);
}

//Funcion para ir adelante
void adelante(){
  velocidad();
  digitalWrite(derdir, HIGH);
  digitalWrite(izqdir,HIGH);
}
//Funcion para ir atras
void atras(){
  velocidad(); 
  digitalWrite(derdir, LOW);
  digitalWrite(izqdir,LOW);
}

//Funcion para rotar derecha
void derecha(){
  velocidad();
  digitalWrite(derdir,HIGH);
  digitalWrite(izqdir,LOW);
}

//Funcion para rotar izquierda
void izquierda(){
  velocidad();
  digitalWrite(derdir,LOW);
  digitalWrite(izqdir,HIGH);
}

void seguidor(){
  int estadoizq=digitalRead(sensorizq);
  int estadoder=digitalRead(sensorder);
  int estadocen=digitalRead(sensorcen);
  
  if(estadocen==0 && estadoizq==1 && estadoder==1){
    adelante();
  }
  else if (estadocen==1 && estadoizq==1 && estadoder==0){
    izquierda();
  }
  else if(estadocen==1 && estadoizq==0 && estadoder==1){
    derecha();
  }
}


