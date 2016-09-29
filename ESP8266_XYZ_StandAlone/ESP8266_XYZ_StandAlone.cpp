#include "ESP8266_XYZ.h"
//define DEBUG

int ESP8266_XYZ::httpPostDirect(String server, String path, int port, String *response){
	//Cálculo de lengitud total del mensaje
	uint16_t json_len = json.length();
	uint16_t rq_len = 120;		//Longitud fija de la solicitud
	rq_len += server.length();
	rq_len += path.length();
	rq_len += json_len;
	rq_len += String(json_len).length();
	rq_len += String(json_len).length();//Duplicado???

	json.setCharAt(json_len-1, '}');

	//Solicitud HTTP al servidor
	//Header
	response += "POST ";
	response += path;
	response += " HTTP/1.1\r\nHost: ";
	response += server;
	response += "\r\nConnection: close\r\n";
	response += "Accept: application/json\r\n";
	response += "Content-Type: application/json\r\nContent-Length:";
	response += json_len;
	response += "\r\n\r\n";
	response += json;
	
	//Comando AT para cerrar la conexión
	//stream->println(F("AT+CIPCLOSE"));

	//Reestablece el JSON para el siguiente POST
	json = "{";

	//Se obtiene el código de estado de la solicitud

	return response;
}