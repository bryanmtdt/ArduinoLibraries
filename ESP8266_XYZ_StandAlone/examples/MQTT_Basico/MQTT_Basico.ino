#include "Arduino.h"

#include "ESP8266_XYZ_StandAlone.h"

const char* wifi_ssid = "SSID";
const char* wifi_pass = "PASS";

#define mqtt_server "IP"
#define mqtt_port 1883

#define mqtt_id "Nodo Prueba"
#define mqtt_retries_reconnect 5
#define mqtt_retries_delay_ms 500


ESP8266_XYZ esp;

#define test_topic "PruebaMQTT"

#define retardo_ms 2000
#define DEBUG 1

int cont=0;

void setup()
{

	if(DEBUG){
		Serial.begin(115200);
		Serial.println("Conectando Wifi...");
	}
	while(!esp.connectAP(wifi_ssid, wifi_pass)){
		delay(200);
		if(DEBUG){
			Serial.println("Conectando Wifi...");
		}
	}

	if(DEBUG){
			Serial.println("MQTT Config");
	}

	esp.MQTTTSetServer(mqtt_server, mqtt_port);
	esp.MQTTConfig(mqtt_id, mqtt_retries_reconnect, mqtt_retries_delay_ms);

	esp.MQTTSetCallback(callbackTest);

	esp.MQTTSubscribe(test_topic);

}

void enviarMensaje(){
	if(DEBUG){
		Serial.println("Enviando Mensaje");
	}
	esp.addToJson("Contador", cont);
	esp.MQTTPublish(test_topic);
	cont++;
}

void loop()
{
	esp.MQTTLoop();
	enviarMensaje();
	delay(retardo_ms);
}

void callbackTest(char* topic, byte* payload, unsigned int length) {
	if(DEBUG){
		Serial.print("Message arrived [");
		Serial.print(topic);
		Serial.print("] ");
		for (uint i = 0; i < length; i++) {
			Serial.print((char)payload[i]);
		}
		Serial.println();
	}
}
