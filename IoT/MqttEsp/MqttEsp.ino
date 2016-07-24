/*
 Ejemplo Básico de MQTT con un ESP8266

 Primeros pasos:
 -Añada el siguiente enlace 
  http://arduino.esp8266.com/stable/package_esp8266com_index.json
  en "File -> Preferences -> Additional Boards Manager URLs"
 -Abra "Tools -> Board -> Board Manager" e instale el ESP8266
 -Seleccione el Soarkfun ESP8266 Thing en "Tools -> Board"
 -Instale la biblioteca PubSubClient en "Sketch -> Manage Libraries"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Valores de las redes inalámbrica y del servidor.
const char* ssid = "FamSaSeq";
const char* password = "AdriKaTa";
const char* mqtt_server = "m12.cloudmqtt.com";
const char* userMQTT = "oiroamoi";
const char* passwordMQTT = "J3wsSmC4kV9O";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);    
  Serial.begin(115200);
  Serial.println("Starting");
  setup_wifi();
  client.setServer(mqtt_server, 14619); //Función para conectarse con el servidor MQTT
  client.setCallback(callback); //Función para escuchar publicaciones del servidor
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Función para escuchar publicaciones del servidor
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

//Ciclo para reconexión
void reconnect() {
 
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    //Intenta conectarse con el servidor
    if (client.connect("ESP8266Client", userMQTT, passwordMQTT)) {
      Serial.println("connected");
      // Si se conecta publica un mensaje
      client.publish("imaginexyz/listen", "AK7");
      // Y se subscribe a un tema
      client.subscribe("imaginexyz/connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {

  // Si no está conectado se intenta conectar
  if (!client.connected()) {
    reconnect();
  }
  
  // Se mantiene enciclado para seguir recibiendo datos
  client.loop();
 
  // Cada 2 segundos se genera un mensaje para publicar en el servidor
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "AK7 #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("imaginexyz/listen", msg);
  }
}
