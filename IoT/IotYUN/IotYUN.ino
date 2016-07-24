#include <Bridge.h> //Biblioteca para comunicar el micro ATMega y el micro con Linux
#include <YunClient.h> //Biblioteca para utilizar llamadas de cliente con el Yun

YunClient client;

String param =""; //String que contendrá los datos a enviar

void setup()
{
  Bridge.begin(); //Iniciar comunicación entre los micros
  Serial.begin(9600); //Iniciar comunicación con el puerto serial

  //Secuencia de solicitud POST al servidor
  if (client.connect("imaginexyz-genuinoday.herokuapp.com", 80)) { //Conexión con el servidor
    Serial.println("connected");
    delay(2500);
    param="temp=HIRVIENDO&estres="+String(20.52); //Datos a enviar
    client.println("POST /imaginexyz/genuinodayb/ HTTP/1.1"); //Tipo de solicitud
    client.println("Host:imaginexyz-genuinoday.herokuapp.com"); //Encabezado especificando el servidor
    client.print("Content-length:"); //Encabezado especificando la longitud
    client.println(param.length());
    Serial.println(param);
    client.println("Content-Type: application/x-www-form-urlencoded"); //Encabezado especificando la forma de la solicitud
    client.println();
    client.println(parampare); //Envío de datos
  }
  else{
    Serial.println("connection failed");
    delay(1000);
  }
  if(client.connected()){
    client.stop();   //Desconectarse del servidor
  }
}

void loop()
{
}
