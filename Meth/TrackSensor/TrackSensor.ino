/*iMA6iNEXYZ
//Curso Explorando el universo robótico
Ejemplo Track Sensor

Este ejemplo servirá como ejemplo para la demostración de: 
  % Declaración de variables
  % Sección de configuración en el código
  % Declaración e Implementación de Funciones
  %Uso de pines digitales
  %Comunicación por puerto serial
  %Uso del cuadro de gráficas del IDLE de arduino
*/

//Inicio del programa

// Declaración de variables
int senal=4; 
int led=13;


//Configuración previa del equipo
//Lo colocado en esta sección solamente se realizará una vez

void setup() {
  //Inicialización del puerto serial
  Serial.begin(9600);
  //Configuración de pines
  pinMode (senal, OUTPUT);
}

//Lo colocado en esta parte del código estará siendo repetido en el MCU muchas veces

//En el caso de este ejemplo se encederá y apagara el LED  de acuerdo a lo detectado en el sensor y se graficará el estado, donde 1 representa si esta encendido o apagado.
//Una vez cargado el programa se debe abrir el Serial Plotter o presionar Ctrl+Mayús+L

void loop() {
  
detectar();

}

//Declaración función detección

void detectar(){
  int estado= digitalRead(senal); 

  if(estado){
  digitalWrite(led,LOW);
  Serial.println(estado);}

  else{
  digitalWrite(led,HIGH);
  Serial.println(estado);}
}

