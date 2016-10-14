#ifndef ESP8266_XYZ_H
#define ESP8266_XYZ_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <string>

class ESP8266_XYZ 
{
	public:
		bool connectAP(const char* ssid, const char* pass);
		void softReset();
		int readResponse(String* response);
		int httpPost(const char* server, String path, int port, String *response);
		int httpGet(const char* server, String path, int port, String *response);
		bool getJsonAttribute(String Input, String Attribute, String *value);
		void addToJson(String id, String value);
		void addToJson(String id, int value);
		void addToJson(String id, float value);
		void setTimeout(uint32_t timeout);
		
	private:
		bool connectServer(const char* server, int port);
		void addToJsonAux(String id, String value);
		WiFiClient client;
		String json = "{";
		int global_timeout = 5000;
};

#endif