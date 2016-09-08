#include "ESP8266_XYZ.h"
#define DEBUG

ESP8266_XYZ::ESP8266_XYZ(Stream *s) 
{
	//Asigna el objeto Stream y el pin de reset del ESP8266
	stream = s;
}

bool ESP8266_XYZ::espTest()
{
	//Comando AT de prueba
	stream->println(F("AT"));
	
	#ifdef DEBUG
		Serial.println(F("AT"));
	#endif
	return findOK();
}

bool ESP8266_XYZ::findOK() {
	delay(100);
	int t = millis();     
	while((millis() - t) < global_timeout){
		if (stream->find("OK"))
		{
		   	return true; 
		}
	}
	return false;               
}

bool ESP8266_XYZ::connectAP(const __FlashStringHelper* ssid, const __FlashStringHelper* pass){
	//Comando AT para el modo de conexión
	stream->println(F("AT+CWMODE=1"));
	findOK();
	//Comando AT para conectarse a una red
	#ifdef DEBUG
	Serial.print(F("AT+CWJAP=\""));
	Serial.print(ssid);
	Serial.print(F("\",\""));
	Serial.print(pass);
	Serial.println(F("\""));
	#endif
	stream->print(F("AT+CWJAP=\""));
	stream->print(ssid);
	stream->print(F("\",\""));
	stream->print(pass);
	stream->println(F("\""));
	delay(1500);
	return findOK();
}

bool ESP8266_XYZ::systemReset(){
	//Comando AT para reiniciar el sistema
	stream->println(F("AT+RST"));

	return findOK();
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


int ESP8266_XYZ::httpPost(String server, String path, int port){

	//Cálculo de lengitud total del mensaje
	uint16_t json_len = json.length();
	uint16_t rq_len = 123;		//Longitud fija de la solicitud
	rq_len += server.length();
	rq_len += path.length();
	rq_len += json_len;
	rq_len += String(json_len).length();

	json.setCharAt(json_len-1, '}');

	#ifdef DEBUG
	Serial.print(F("AT+CIPSTART=\"TCP\",\"http://"));
	Serial.print(server);
	Serial.print(F("\","));
	Serial.println(port);
	#endif

	//Comando AT para abrir la conexión
	stream->print(F("AT+CIPSTART=\"TCP\",\""));
	stream->print(server);
	stream->print(F("\","));
	stream->println(port);

	delay(300);

	//Serial.println(stream->readStringUntil('\n'));

	if(findOK()){
		Serial.println("Connected to server");

	} else {
		Serial.println("Connection Failure");
	}

	//Comando AT para comunicarse con el servidor
	#ifdef DEBUG
	Serial.print(F("AT+CIPSEND="));
	Serial.println(rq_len);
	#endif

	stream->print(F("AT+CIPSEND="));
	stream->println(rq_len);

	server += ":";
	server += String(port);
	delay(100);
	stream->find(">");
	delay(300);

	//Solicitud HTTP al servidor
	//Header
	#ifdef DEBUG
	Serial.print(F("POST "));
	Serial.print(path);
	Serial.print(F(" HTTP/1.1\r\nHost: "));
	Serial.print(server);
	Serial.print(F("\r\nConnection: close\r\n"));
	Serial.print(F("Accept: application/json\r\n"));
	Serial.print(F("Content-Type: application/json\r\nContent-Length:"));
	Serial.print(json_len);
	Serial.println(F("\r\n"));
	#endif
	stream->print(F("POST "));
	stream->print(path);
	stream->print(F(" HTTP/1.1\r\nHost: "));
	stream->print(server);
	stream->print(F("\r\nConnection: close\r\n"));
	stream->print(F("Accept: application/json\r\n"));
	stream->print(F("Content-Type: application/json\r\nContent-Length:"));
	stream->print(json_len);
	stream->print(F("\r\n\r\n"));

	//Body
	#ifdef DEBUG
	Serial.println(json);
	#endif

	stream->println(json);
	//stream->print(F("\r\n\r\n"));

	delay(1000);

	String resp;

	//Se obtiene el código de estado de la solicitud
	int statusCode = readResponse(&resp);



	//Comando AT para cerrar la conexión
	#ifdef DEBUG
	Serial.print("Srv: ");
	Serial.println(resp);
	//Serial.println(F("AT+CIPCLOSE"));
	#endif
	//stream->println(F("AT+CIPCLOSE"));

	//Reestablece el JSON para el siguiente POST
	json = "{";

	return statusCode;
}

int ESP8266_XYZ::httpGet(String server, String path, int port, String *response){

	//Cálculo de lengitud total del mensaje
	uint16_t rq_len = 25;		//Longitud fija de la solicitud
	rq_len += server.length();
	rq_len += path.length();

	//Comando AT para abrir la conexión
	stream->print(F("AT+CIPSTART=\"TCP\",\"http://"));
	stream->print(server);
	stream->print(F("\","));
	stream->println(port);

	//Comando AT para comunicarse con el servidor
	stream->print(F("AT+CIPSEND="));
	stream->println(rq_len);

	server += ":";
	server += String(port);

	delay(100);

	//Solicitud HTTP al servidor
	//Header
    stream->print(F("GET "));
    stream->print(path);
    stream->print(F(" HTTP/1.1\r\nHost: "));
   	stream->print(server);
	stream->print(F("\r\n\r\n"));

	delay(100);

	//Se obtiene el código de estado de la solicitud
	int statusCode = readResponse(response);

	//Comando AT para cerrar la conexión
	//stream->println(F("AT+CIPCLOSE"));

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

/*bool ESP8266_XYZ::getJsonAttribute(String attribute, String* value) {

	//Banderas de control del parseo
	bool found_attribute = false;
	bool in_value = false;
	bool in_attribute = false;

	//Variables de soporte
	String att_buffer = "";
	String val_buffer = "";
	int code = 0;
	uint32_t t = millis();

	//Se lee caracter por caracter de acuerdo con las distintas partes
	//de un response HTTP
	for(int i = 0; i < ; i++) {


    	char c = 

	    //Luego de comillas se encuentra un nombre de atributo
	    if(c == '\"'){
	    	in_attribute = !in_attribute;
	    }

	    //Se concatena al buffer de atributo 
	    if(c != '\"' && in_attribute){
	    	att_buffer += c;
	    }

	 	if(c == ':'){
	    	in_value = true;
	    }

	    //Se concatena al buffer de atributo 
	    if(c != ':' && c != ',' && in_value){
	    	val_buffer += c;
	    }

	    if(c == ','){
	    	in_value = false;
	    	if(attribute.compare(att_buffer) == 0){
	    		value->assign(val_buffer);
	    	} else {
	    		val_buffer = "";
	    		att_buffer = "";
	    	}
	    }
	}

  return code;
}*/