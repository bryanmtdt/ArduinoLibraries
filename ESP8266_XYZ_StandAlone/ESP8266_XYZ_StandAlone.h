#ifndef ESP8266_XYZ_H
#define ESP8266_XYZ_H

#include <Arduino.h>
//#include <string>

class ESP8266_XYZ 
{
	public:
		ESP8266_XYZ(Stream *s = &Serial, int rst = -1);
		
		int httpPostDirect(String server, String path, int port, String *response);
		
		
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