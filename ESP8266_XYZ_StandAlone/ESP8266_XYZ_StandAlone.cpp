#include "ESP8266_XYZ_StandAlone.h"
//#define DEBUG

bool ESP8266_XYZ::connectAP(const char* ssid, const char* pass){
	uint32_t t = millis();
	WiFi.begin(ssid, pass);
  	while (WiFi.status() != WL_CONNECTED && (millis() - t) < global_timeout) {
    	delay(100);
  	}
  	return WiFi.status(); 
}

void ESP8266_XYZ::softReset(){
	ESP.restart();
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
	while ((millis() - t) < global_timeout) {

	    if (client.available()) {

	    	char c = client.read();;
	    	

		    //Luego del primer espacio se encuentra el código de estado
		    if(c == ' ' && !in_status){
		        in_status = true;
		    }

		    //Se lee el código de estado de 3 dígitos
		    if(in_status && i < 3 && c != ' '){////Qué pasa si el código es de 4 digitos???
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
	response->remove(0,response->indexOf('{'));
	response->remove(response->indexOf('}')+1);

	#ifdef DEBUG 
	    	Serial.println(*response);
	#endif
  	return code;
}


bool ESP8266_XYZ::connectServer(const char* server, int port){
	uint32_t t = millis();
	bool connected = false;
	do {
    	connected = client.connect(server, port);
    	delay(500);
  	} while (!connected && (millis() - t) < global_timeout);
  	return connected;
}


int ESP8266_XYZ::httpPost(const char* server, String path, int port, String *response){
	String msg = "";
	String server_str = server;
	if(connectServer(server, port)){
		#ifdef DEBUG 
			Serial.println("Connected to server");
		#endif

	} else {
		#ifdef DEBUG 
			Serial.println("Connection Failure");
		#endif
		return -1;
	}

	server_str += ":";
	server_str += String(port);

	//Cálculo de lengitud total del mensaje
	uint16_t json_len = json.length();

	json.setCharAt(json_len-1, '}');

	//POST  HTTP/1.1\r\nHost: \r\nConnection: close\r\nAccept: application/json\r\nContent-Type: application/json\r\nContent-Length:\r\n\r\n

	//Solicitud HTTP al servidor
	//Header
	msg += "POST ";
	msg += path;
	msg += " HTTP/1.1\r\nHost: ";
	msg += server_str;
	msg += "\r\nConnection: close\r\n";
	msg += "Accept: application/json\r\n";
	msg += "Content-Type: application/json\r\nContent-Length:";
	msg += json_len;
	msg += "\r\n\r\n";
	msg += json;

	#ifdef DEBUG 
			Serial.print("Request: ");
			Serial.println(msg);
	#endif
	
	client.print(msg);
	//Reestablece el JSON para el siguiente POST
	json = "{";

	return readResponse(response);
	
}

int ESP8266_XYZ::httpGet(const char* server, String path, int port, String *response){
	String msg = "";
	String server_str = server;

	if(connectServer(server, port)){
		#ifdef DEBUG 
			Serial.println("Connected to server");
		#endif

	} else {
		#ifdef DEBUG
			Serial.println("Connection Failure");
		#endif
	}

	server_str += ":";
	server_str += String(port);

	//GET  HTTP/1.1\r\nHost: \r\n\r\n
	//Solicitud HTTP al servidor
	//Header
    msg += "GET ";
    msg += path;
    msg += " HTTP/1.1\r\nHost: ";
   	msg += server_str;
	msg += "\r\n\r\n";

	//Se envía el mensaje al servidor
	client.print(msg);

	//Se obtiene el código de estado de la solicitud
	return readResponse(response);
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

void ESP8266_XYZ::addToJsonAux(String id, String value){
	json += "\"";
	json += id;
	json += "\":";
	json += value;
	json += ",";
}

void ESP8266_XYZ::addToJson(String id, int value){
	addToJsonAux(id, String(value));
}

void ESP8266_XYZ::addToJson(String id, float value){
	addToJsonAux(id, String(value));
}

bool ESP8266_XYZ::getJsonAttribute(String Input, String Attribute, String *value) {

	int start = Input.indexOf(":",Input.indexOf(Attribute));
	int end = Input.indexOf(",",start);
	int end2 = Input.indexOf("}",start);
	if (end == -1){end = end2;}
	for (int n = start+1; n<end; n++) {
		value -> concat(Input[n]);
	}
	return value;               
}