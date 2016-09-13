#ifndef ESP8266_XYZ_H
#define ESP8266_XYZ_H

#include <Arduino.h>
//#include <string>

class ESP8266_XYZ 
{
	public:
		ESP8266_XYZ(Stream *s = &Serial, int rst = -1);
		bool espTest();
		bool find_serial(int ser_timeout, String str);
		bool serial_line(int i, String str);
		bool connectAP(const __FlashStringHelper *ssid, const __FlashStringHelper *pass);
		bool softReset();
		bool hardReset();
		int readResponse(String* response);
		int httpPost(String server, String path, int port);
		int httpGet(String server, String path, int port, String *response);
		bool getJsonAttribute(String attribute, String *value);
		void addToJson(String id, String value);
		void addToJson(String id, int value);
		void addToJson(String id, float value);
		void setTimeout(uint32_t timeout);
		
	private:
		void readSerialContent(int ser_timeout);
		bool connectServer(String server, int port);
		void addToJsonAux(String id, String value);
		int rst;
		String json = "{";
		String serial_content = "";
		Stream *stream;
		int global_timeout = 5000;

};

#endif