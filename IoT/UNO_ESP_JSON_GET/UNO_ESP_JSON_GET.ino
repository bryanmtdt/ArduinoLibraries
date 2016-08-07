/*
   El ESP8266 debe estar pre-configurado a 115200 Baudios
   Conectar el ESP8266 de acuerdo al diagrama UNO_ESP_JSON
*/

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); //Pines: RX conectado a D3, TX conectado a D2

//
//#define DEBUG_ESP8266 //Comentar si no se quiere imprimir la respuesta del ESP8266
//#define ESP8266_OK //Confirmar si el comando AT fue recibido

#define SSID   "Museocr"
#define PASS   "museocr1"

String server = "AT+CIPSTART=\"TCP\",\"https://hwthoncr16.herokuapp.com\",80";  //Direccion del servidor al que se envía la solicitud
String getl = "GET /centrophorus/last HTTP/1.1\r\nHOST: hwthoncr16.herokuapp.com:80\r\n\r\n";
String trama; // Almacena el comando AT que envía el largo del dato a enviarse al servidor

//-----------------------------------------------------------------------------------------------------------------
void setup() {

  Serial.begin(115200);             // Inicializacion del Monitor Serial a 115200
  mySerial.begin(38400);           // Inicializacion  puerto serial virtual
  Serial.println(F("Manejo Modulo ESP8266 con Arduino UNO"));// Mensaje de inicialización

  //*********************Pruebas iniciales**************************
  Serial.println("AT");
  SendCmd("AT", 60);
  Serial.println("ATE1");
  SendCmd("ATE1", 60); //ATE1: Habilita replica/echo del cmd enviado
  Serial.println("ATE0");
  SendCmd("ATE0",60); //ATE0: Deshabilita replica/echo del cmd enviado
  Serial.println("AT+UART");
  SendCmd("AT+UART=38400,8,1,0,0", 60);

  conectarWIFI();                 // Función para conectar el módulo ESP8266 a la red WiFi selecccionada
  delay(20);
}
//****************************************** FUNCIONES****************************************************************//

void conectarWIFI() {       // Función que permite conectarse al servidor específicado en el string server
  Serial.println("AT+CWMODE");
  SendCmd("AT+CWMODE=3", 60);
  Serial.println("AP");
  String AP = "AT+CWJAP=\""; AP += SSID; AP += "\",\""; AP += PASS; AP += "\""; // Comando AT para la conexión WiFi seleccionada con el SSID y PASS
  SendCmd(AP, 60);
  Serial.println("AT");
  SendCmd("AT", 60);
}

void resetESP() {
  Serial.println("RST");
  mySerial.println("AT+RST");        // Deshabilita el echo de los comandos enviados
  delay(20);
  conectarWIFI();                 // Función para conectar el módulo ESP8266 a la red WiFi selecccionada
}

void leer() {
  while (mySerial.available()) {
    String recibido = "";
    if (mySerial.available()) {
      recibido += (char)mySerial.read();
    }
    Serial.print(recibido);
  }
}

bool findOK() {                     //Función que permite verificar el resultado "OK" del comando AT
  String str = mySerial.readStringUntil('\n');
  if (str.indexOf("OK") != -1)         // Si se localiza OK en la respuesta del ESP8266
  {
    Serial.println("OK");
    return true;                    // Devuelve "True"
  }
  else
  {
    //Serial.println("!OK");
    return false;                   // Retorna "False"
  }
}

void SendCmd (String ATcmd, int Tespera) {
  mySerial.println(ATcmd);
  delay(100); //Tiempo a esperar para abrir el puerto mySerial

}

void loop() {

  //*****************Conexión con el servidor*************************
  Serial.println(getl);
  trama = "AT+CIPSEND=" + String(getl.length());
  SendCmd(server, 60);
  mySerial.println(trama);
  delay(100);
  mySerial.println(getl);
  delay(300);
  delay(300);
  String recibido = "";
  //////////////////////////////
  while (mySerial.available()) {
    
    recibido += mySerial.readStringUntil('\n');;
    
  }
  Serial.println("-------------------------");
  Serial.println(recibido);
  Serial.println("+++++++++++++++++++++++++");
  //resetESP();
}
