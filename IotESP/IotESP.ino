#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); //Pines: RX->D3, TX->D2
char a;
String data;
String rst = "AT+RST"; //Comando AT para reiniciar el ESP
String modo = "AT+CWMODE=3"; //Comando AT para establecer el ESP en modo AP
String conectar = "AT+CWJAP=\"SSID\",\"PASS\""; //Comando AT para conectarse a una red inalámbrica
String server = "AT+CIPSTART=\"TCP\",\"https://imaginexyz-genuinoday.herokuapp.com\",80"; //Comando AT para establecer conexión con el servidor
String trama; //Variable donde se almacenará el comando AT para establecer la longitud del mensaje
String post = "POST /imaginexyz/genuinodayq/?dato1=imagineXYZ&dato2=ROCKEA HTTP/1.1\r\nHost: imaginexyz-genuinoday.herokuapp.com:80\r\n\r\n"; //Secuencia de solicitud POST
String POSTA = "POST /imaginexyz/genuinodayb/ HTTP/1.1\r\nHost: imaginexyz-genuinoday.herokuapp.com:80\r\nAccept: application/json\r\nContent-Type: application/json\r\nContent-Length: ";
void setup() {
  Serial.begin(9600); //Inicializar puerto serial
  mySerial.begin(9600); //Inicializar puerto de comunicación con el ESP
  Serial.println("prueba");
  mySerial.println("AT"); //Mensaje "Prueba" al ESP
  leer(); //Leer datos devueltos por el ESP

  //Los siguientes comandos deben ejecutarse sólo la primera vez
  mySerial.println(rst); //Reiniciar ESP
  leer();
  
  mySerial.println(modo); //Establecer modo del ESP
  leer();
  
  mySerial.println(conectar); //Conectarse con la red
  leer();

  //Estos comandos se deben ejecutar cada vez que se desee comunicarse con el servidor
  mySerial.println(server); //Establecer conexión
  leer();
  
  trama = "AT+CIPSEND=" + String(post.length()); //Comando AT para establecer la longitud del mensaje 
  mySerial.println(trama); //Establecer longitud
  leer();

  mySerial.println(post); //Enviar solicitud de POST
  leer();
}

//Función para leer los datos enviados por el ESP
void leer() {
  unsigned long start = millis();
  while (millis() - start < 10000) {
    while (mySerial.available()>0){
      a = mySerial.read();
      if (a=='\0') continue;
      data += a;
    }    
  }
  Serial.print(data);
  data = "";
}

bool findOK(){                      //Función que permite verificar el resultado "OK" del comando AT 
    if(Serial1.find("OK"))          // Si se localiza OK en la respuesta del ESP8266
    {
    SerialUSB.println("Server OK"); // El comando se ejecutó correctamente
    return true;                    // Devuelve "True"
     }
    else
    {
    SerialUSB.println("No Server"); // Si ha ocurrido un error
    return false;                   // Retorna "False"
     }
}


void loop() {
  // put your main code here, to run repeatedly:
}
