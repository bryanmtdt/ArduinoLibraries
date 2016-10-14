#include "ESP8266_XYZ.h"
//define DEBUG



void ESP8266_XYZ::readSerialContent(int ser_timeout){
	delay(ser_timeout);
	while(stream->available()){
		serial_content += char(stream->read());
		//delay(10);
	}
	#ifdef DEBUG
		Serial.println(serial_content);
	#endif
}

bool ESP8266_XYZ::espTest()
{
	//Comando AT de prueba
	stream->println(F("AT"));
	
	return find_serial(100, "OK");
}

bool ESP8266_XYZ::find_serial(int ser_timeout, String str) {
	readSerialContent(ser_timeout);
	if (serial_content.indexOf(str) != -1)
	{	
		serial_content = "";
	   	return true; 
	}
	return false;               
}

bool ESP8266_XYZ::serial_line(int i, String str) {///todo esto depende del baudrate 
	delay(200);
	int k = 0;
	while(k < i ){
		String line = stream->readStringUntil('\n'); 
		if (line.indexOf(str) != -1){
			#ifdef DEBUG 	
				Serial.println(line);
			#endif
	   		return true; 
		}
		delay(50);
		k++;
	}
	return false;
}

bool ESP8266_XYZ::connectAP(const __FlashStringHelper* ssid, const __FlashStringHelper* pass){
	//Comando AT para el modo de conexión
	stream->println(F("AT+CWMODE=3"));//Modo 3 de operación: SoftAP + Station 
	find_serial(100, "OK");
	//Comando AT para conectarse a una red


	//Hay que hacer una rutina que pregunte si ya está conectado, para que no se conecte simepre que arranaca
	//por default cuando enciende se conecta al ultimo ap configurado
	//Si no se puede configurar también (AT+CIFSR)

	stream->println(F("AT+CWQAP"));
	readSerialContent(100);
	stream->print(F("AT+CWJAP=\""));
	stream->print(ssid);
	stream->print(F("\",\""));
	stream->print(pass);
	stream->println(F("\""));

	//readSerialContent();
	return find_serial(7000, "GOT IP");
}

bool ESP8266_XYZ::softReset(){
	//Comando AT para reiniciar el sistema
	stream->println(F("AT+RST"));

	return find_serial(100, "OK");
}

bool ESP8266_XYZ::hardReset() {
	pinMode(rst, OUTPUT); 
  	if(rst < 0) {
  		return false;
  	}
	digitalWrite(rst, LOW);
	delay(10);                 
	digitalWrite(rst, HIGH);
	return find_serial(100, "OK");   
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



bool ESP8266_XYZ::connectServer(String server, int port){

	//stream->println(F("AT+CIFSR"));
	//readSerialContent(150);

	//Comando AT para abrir la conexión
	stream->print(F("AT+CIPSTART=\"TCP\",\""));
	stream->print(server);
	stream->print(F("\","));
	stream->println(port);


	return find_serial(5000, "OK");
	//return findOK(7000);
}


int ESP8266_XYZ::httpPost(String server, String path, int port, String *response){
	

	stream->println(F("AT+CIPCLOSE"));
	readSerialContent(100);
	

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

	server += ":";
	server += String(port);

	//Cálculo de lengitud total del mensaje
	uint16_t json_len = json.length();
	uint16_t rq_len = 120;		//Longitud fija de la solicitud
	rq_len += server.length();
	rq_len += path.length();
	rq_len += json_len;
	rq_len += String(json_len).length();

	json.setCharAt(json_len-1, '}');

	delay(250);
	stream->print(F("AT+CIPSEND="));
	stream->println(rq_len);
	find_serial(300, ">");

//POST  HTTP/1.1\r\nHost: \r\nConnection: close\r\nAccept: application/json\r\nContent-Type: application/json\r\nContent-Length:\r\n\r\n

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
	stream->print(F("\r\n\r\n"));

	stream->println(json);
	
	//Comando AT para cerrar la conexión
	//stream->println(F("AT+CIPCLOSE"));

	//Reestablece el JSON para el siguiente POST
	json = "{";

	//Se obtiene el código de estado de la solicitud

	if(serial_line(10, "SEND OK")){
		return readResponse(response);
	} else {
		return -1;
	}

}

int ESP8266_XYZ::httpGet(String server, String path, int port, String *response){

	stream->println(F("AT+CIPCLOSE"));
	readSerialContent(100);
	if(connectServer(server, port)){
		#ifdef DEBUG 
			Serial.println("Connected to server");
		#endif

	} else {
		#ifdef DEBUG
			Serial.println("Connection Failure");
		#endif
	}

	server += ":";
	server += String(port);

	//Cálculo de lengitud total del mensaje
	uint16_t rq_len = 25;		//Longitud fija de la solicitud
	rq_len += server.length();
	rq_len += path.length();
	delay(250);
	
	//Comando AT para comunicarse con el servidor
	stream->print(F("AT+CIPSEND="));
	stream->println(rq_len);
	find_serial(300, ">");

	//GET  HTTP/1.1\r\nHost: \r\n\r\n
	//Solicitud HTTP al servidor
	//Header
    stream->print(F("GET "));
    stream->print(path);
    stream->print(F(" HTTP/1.1\r\nHost: "));
   	stream->print(server);
	stream->print(F("\r\n\r\n"));

	//Comando AT para cerrar la conexión
	//stream->println(F("AT+CIPCLOSE"));

	//Se obtiene el código de estado de la solicitud
	if(serial_line(10, "SEND OK")){
		return readResponse(response);
	} else {
		return -1;
	}
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

ESP8266_XYZ::ESP8266_XYZ(Stream *s, int rst_pin) 
{
	//Asigna el objeto Stream y el pin de reset del ESP8266
	stream = s;
	rst = rst_pin;
	//hardReset();
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