#ifndef ESP8266_XYZ_H
#define ESP8266_XYZ_H

#include <Arduino.h>
//#include <string>

class ESP8266_XYZ 
{
	public:
		ESP8266_XYZ(Stream *s = &Serial);
		bool espTest();
		bool findOK();
		bool connectAP(const __FlashStringHelper *ssid, const __FlashStringHelper *pass);
		bool systemReset();
		int readResponse(String* response);
		int httpPost(String server, String path, int port);
		int httpGet(String server, String path, int port, String *response);
		bool getJsonAttribute(String attribute, String *value);
		void addToJson(String id, String value);
		void addToJson(String id, int value);
		void addToJson(String id, float value);
		void setTimeout(uint32_t timeout);
		
	private:
		void addToJsonAux(String id, String value);
		String json = "{";
		Stream *stream;
		int global_timeout = 5000;

};

#endif