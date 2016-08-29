#ifndef ESP8266_XYZ_H
#define ESP8266_XYZ_H

#include <Arduino.h>

class ESP8266_XYZ 
{
	public:
		ESP8266_XYZ(Stream *s = &Serial, int8_t r = -1);
		bool espTest();
		bool findOK();
		bool connectAP(__FlashStringHelper ssid, __FlashStringHelper pass);
		bool systemReset();
		int httpPost(String server, String path, int port);
		int httpGet(String server, String path, int port, String *response);
		void addToJson(String id, String value);
		void addToJson(String id, int value);
		void setTimeout(uint32_t timeout);
		
	private:
		String json = "{";
		Stream *stream;
		int reset;
		int gobal_timeout;

}

#endif