#include "ESP8266_XYZ.h"

ESP8266_XYZ::ESP8266_XYZ(Stream *s, int8_t rst) 
{
	//Asigna el objeto Stream y el pin de reset del ESP8266
	stream = s;
	reset = rst;
}

bool ESP8266_XYZ::espTest()
{
	//Comando AT de prueba
	stream->println("AT");
	return findOK();
}

bool ESP8266_XYZ::findOK() {
	uint32_t t = millis();     
	while(millis() - t < timeout){
		String str = stream->readStringUntil('\n');
		if (str.indexOf("OK") != -1)
		{
		   	return true; 
		}
	}
	return false;               
}

bool ESP8266_XYZ::connectAP(__FlashStringHelper ssid, __FlashStringHelper pass){
	//Comando AT para el modo de conexión
	stream->println(F("AT+CWMODE=3"));

	//Comando AT para conectarse a una red
	String ap = "AT+CWMJAP=\"";
	ap += ssid;
	ap += "\",\"";
	ap += pass;
	ap += "\"";
	stream->println(ap);

	return findOK();
}

bool ESP8266_XYZ::systemReset(){
	//Comando AT para reiniciar el sistema
	stream->println(F("AT+RST"));

	return findOK();
}

int ESP8266_XYZ::httpPost(String server, String path, int port){

	//Cálculo de lengitud total del mensaje
	uint16_t json_len = strlen(json);
	uint16_t rq_len = 122;		//Longitud fija de la solicitud
	rq_len += strlen(server);
	rq_len += strlen(path);
	rq_len += json_len;
	rq_len += strlen(tostring(json_len));

	json.at(json_len-1) = '}';

	//Comando AT para abrir la conexión
	String cmd = "AT+CIPSTART=\"TCP\",\"https://"
	cmd += server;
	cmd += "\","
	cmd += tostring(port);
	stream->println(cmd);

	//Comando AT para comunicarse con el servidor
	cmd = "AT+CIPSEND=";
	cmd += tostring(rq_len);
	stream->println(cmd);

	server += ":";
	server += tostring(port);

	delay(100);

	//Solicitud HTTP al servidor
	//Header
    stream->print(F("POST "));
    stream->print(path);
    stream->print(F(" HTTP/1.1\r\nHost: "));
   	stream->print(server);
    stream->print(F("\r\nConnection: close\r\n"));
    stream->print(F("Accept: application/json\r\n"));
	stream->print(F("Content-Type: application/json\r\nContent-Length:"));
	stream->print(json_len);
	stream->print("\r\n");

	//Body
	stream->print(json);
	stream->print("\r\n\r\n");

	delay(100);

	//Se obtiene el código de estado de la solicitud
	int statusCode = readResponse(NULL);

	//Comando AT para cerrar la conexión
	stream->println(F("AT+CIPCLOSE"));

	return statusCode;
}

int ESP8266_XYZ::httpGet(String server, String path, int port, String *response){

	//Cálculo de lengitud total del mensaje
	uint16_t json_len = strlen(json);
	uint16_t rq_len = 25;		//Longitud fija de la solicitud
	rq_len += strlen(server);
	rq_len += strlen(path);

	//Comando AT para abrir la conexión
	String cmd = "AT+CIPSTART=\"TCP\",\"https://"
	cmd += server;
	cmd += "\","
	cmd += tostring(port);
	stream->println(cmd);

	//Comando AT para comunicarse con el servidor
	cmd = "AT+CIPSEND=";
	cmd += tostring(rq_len);
	stream->println(cmd);

	server += ":";
	server += tostring(port);

	delay(100);

	//Solicitud HTTP al servidor
	//Header
    stream->print(F("GET "));
    stream->print(path);
    stream->print(F(" HTTP/1.1\r\nHost: "));
   	stream->print(server);
	stream->print("\r\n\r\n");

	delay(100);

	//Se obtiene el código de estado de la solicitud
	int statusCode = readResponse(response);

	//Comando AT para cerrar la conexión
	stream->println(F("AT+CIPCLOSE"));

	return statusCode;
}

void ESP8266_XYZ::setTimeout(uint32_t timeout){
	global_timeout = timeout;
}

void ESP8266_XYZ::addToJson(String id, String value){
	json += "\"";
	json += id;
	json += "\":\"";
	json += value;
	json += "\",";
}

void ESP8266_XYZ::addToJson(String id, int value){
	json += "\"";
	json += id;
	json += "\":";
	json += tostring(value);
	json += ",";
}

int ESP8266_XYZ::readResponse(String* response) {

	//Banderas de control del parseo
	bool line_blank = true;
	bool http_body = false;
	bool in_status = false;

	//Variables de soporte
	char status_code[4];
	int i = 0;
	int code = 0;
	uint32_t t = millis();

	//Se lee caracter por caracter de acuerdo con las distintas partes
	//de un response HTTP
	while (millis() - t < timeout) {

	    if (stream->available()) {

	    	char c = stream->read();

		    //Luego del primer espacio se encuentra el código de estado
		    if(c == ' ' && !in_status){
		        in_status = true;
		    }

		    //Se lee el código de estado de 3 dígitos
		    if(in_status && i < 3 && c != ' '){
		        status_code[i] = c;
		        i++;
		    }

		    //Se parsea el código de char* a int
		    if(i == 3){
		        status_code[i] = '\0';
		        code = atoi(status_code);
		    }

		    if(http_body){
		    	//Si se detecta el cuerpo de la respuesta
		    	//se escribe en response (si existe)
		   		if(response != NULL) response->concat(c);
		    } else {	
		    	//Condiciones para detectar el cuerpo de la respuesta
		        if (c == '\n' && line_blank) {
		            http_body = true;
		        }
		        if (c == '\n') {
		            line_blank = true;
		        } else if (c != '\r') {
		            line_blank = false;
		        }
		    }
	    }
	}

  return code;
}