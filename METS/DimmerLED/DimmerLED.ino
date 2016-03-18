/*iMA6iNEXYZ
//Curso Explorando el universo robótico
Ejemplo BlinkLED

Este ejemplo servirá como ejemplo para la demostración de: 
  % Declaración de variables
  % Sección de configuración en el código
  % Declaración e Implementación de Funciones
  %Uso de señales analógicas y PWM
  %Comunicación por puerto serial
  %Uso del cuadro de gráficas del IDLE de arduino
  

 */

//Declaración de variables

int led = 9;         // PIN de PWM donde realizará el control del pin
int brillo = 0;     // brillo que tiene el LED
int dimmer = 4;    // Puntos de brillo que tiene
int del= 30;      // Tiempo de espera para la iteración

// Configuración que sólo se realiza una vez
void setup() {
  //Inicialización del puerto serial
  Serial.begin(9600);
  //Configuración del LED como salida
  pinMode(led, OUTPUT);
}

//Lo colocado en esta parte del código estará siendo repetido en el MCU muchas veces

//En el caso de este ejemplo se dimerizará el LED y la señal estará graficada.
//Una vez cargado el programa se debe abrir el Serial Plotter o presionar Ctrl+Mayús+L

void loop() {
  //Brillo que tendrá el LED
  analogWrite(led, brillo);
  //Gráfico de datos actual
  Serial.println(brillo);

  // Cambio del brillo, en la siguiente iteración del código
  brillo = brillo + dimmer;

  //Invertir el dimmer cuando se alcanza una condición frontera:
  if (brillo == 0 || brillo == 255) {
    dimmer = -dimmer ;
  }
  // Tiempo de espera para que se realice la siguiente etapa
  delay(del);
}
