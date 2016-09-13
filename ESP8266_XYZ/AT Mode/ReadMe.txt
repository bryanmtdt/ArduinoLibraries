**Primeros pasos para utilizar UNO_ESP_JSON**

Configuración de fábrica: Cada ESP8266 dependiendo del fabricante tiene una tasa de transmisión de datos (baudios) diferente.
Para configurar los baudios de manera correcta, realizar los siguientes pasos:

1. Conectar el ESP8266 y Arduino UNO como lo muestra el diagrama en "Primero_ESP8266"
2. Cargar el ejemplo "Blink" al Arduino UNO.
3. Abrir monitor serial en 9600 baudios CON LA CONFIGURACIÓN DE "Both NL & CR" y digitar "AT" en la barra de envío.
4. Comprobar que el ESP8266, responda "OK", debería verse así:

AT

OK

5. Si lo anterior sucede está preparad@ para usar UNO_ESP_JSON. 

Caso contrario cambiar los baudios en el monitor serial y digitar "AT" hasta encontrar "OK". El orden puede ser el siguiente:


- 9600
- 19200
- 38400
- 57600
- 74880
- 115200

6. Si responda "OK", digitar el siguiente comando: "AT+UART=9600,8,1,0,0"
7. Cambiar nuevamente los baudios a 38400 y comprobar que el cambio fue correcto digitando nuevamente "OK":

AT

OK

NOTA: Cada vez que que envía un comando al ESP8266 o recibe un comando del ESP8266, se debe encender un LED (típicamente azul) en el shield del ESP8266. Si esto no sucede revisar las conexiones entre los pines 0 y 1 del Arduino UNO y las conexiones de RX y TX del ESP8266.

Puede ser confuso la interconexión entre pines del puerto UART, se debe entender así:

______________________________								______________________________
UNO 													ESP8266
                                      0->RX
Pin 0: Transmite información ------------->>>>>>>>>>>   Pin RXD: Recibe información

                                      1<-TX
Pin 1: Recibe información    <<<<<<<<<<<<<-----------   Pin TXD: Transmite información


_____________________________________________________
PINOUT:

RXD		VCC

GPIO0		RST	

GPIO2		CH_PD
	
GND		TXD










