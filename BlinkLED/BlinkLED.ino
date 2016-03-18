/*iMA6iNEXYZ
//Curso Explorando el universo robótico
Ejemplo BlinkLED

Este ejemplo servirá como ejemplo para la demostración de: 
  % Declaración de variables
  % Sección de configuración en el código
  % Declaración e Implementación de Funciones
  %Uso de pines digitales
  %Comunicación por puerto serial
  %Uso del cuadro de gráficas del IDLE de arduino
*/

//Inicio de programa

// Declaración de variables
int led=13; 
int espera=1000;

//Configuración previa del equipo
// Lo colocado en esta sección, solamente se ejecutará una vez. 

void setup() {

  //Inicialización de puerto serial
  Serial.begin(9600);
  //Configuración de pines
  pinMode(led, OUTPUT);
  
}

//Lo colocado en esta parte del código estará siendo repetido en el MCU muchas veces

//En el caso de este ejemplo se encederá y apagara el LED y se graficará el estado, donde 1 representa si esta encendido o apagado.
//Una vez cargado el programa se debe abrir el Serial Plotter o presionar Ctrl+Mayús+L

void loop() {
    digitalWrite(13, HIGH);
    for(int i=0; i<10; i++){
      Serial.println(1); }
    delay(espera);
    digitalWrite(13, LOW);
    for(int j=0; j<10; j++){
      Serial.println(0);  }
    delay(espera);  
    
}

//Fin del programa



