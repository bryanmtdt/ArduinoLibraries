Uso esptool.py para quemar firmware AT version

0. Instalar https://github.com/espressif/esptool

1. Ejecutar esptool.py localizado en usr/local/bin 
   Es el default de ejecutables en python

2. En el caso de los ESP8266 de 512kB (azules) se debe usar a versión: 
ESP_8266_BIN0.92.bin

ESP8266 modernos: Obtener el firmware más actualizado de: http://www.electrodragon.com/w/ESP8266_AT-Command_firmware

3. Seleccionar los tamaños de memoria y baudrate del ESP8266

El azul: Generic ESP8266 Module, 80 MHz, 40 MHz, DIO, 11500, 512k (64k SPIFFS)
El negro: Generic ESP8266 Module, 80 MHz, 40 MHz, DIO, 11500, 1M (512k SPIFFS)
Anotar el puerto /dev/ttyUSB0

4. Asegurar que los pines RX y TX estén bien conectados, que el FTDI comparta la tierra con el ESP8266 y que el ESP8266 esté alimentado con 3.3 V (idelamente de una fuente diferente del FTDI)

5. Modo bootloader: Dejar presionado RST, luego dejar presionado GPIO0, liberar reset, y por ultimo liberar GPIO0

6. Desde consola navegar a /usr/local/bin default folder for python

7. esptool.py --port /dev/ttyUSB0 write_flash --flash_mode dio --flash_size 4m --flash_freq 40m --verify 0x0000 ~/Downloads/AT\ Firmware\ ESP8266/Old/ESP8266_flasher_V00170901_00_Cloud\ Update\ Ready/ESP_8266_BIN0.92.bin 

Referencia>

--port /dev/ttyUSB0 -> puerto donde se encuentra conectado (Windows COMX)
write_flash --flash_mode dio -> tipo de datos 
	    --flash_size 4m 
	    --flash_freq 40m 
	    --verify 
	    0x0000

7. Resultado:

esptool.py v1.3
Connecting....
Running Cesanta flasher stub...
Flash params set to 0x0200
Wrote 520192 bytes at 0x0 in 45.1 seconds (92.3 kbit/s)...
Leaving...
Verifying just-written flash...
Flash params set to 0x0200
Verifying 0x7f000 (520192) bytes @ 0x00000000 in flash against /home/carvajavo/Downloads/AT Firmware ESP8266/Old/ESP8266_flasher_V00170901_00_Cloud Update Ready/ESP_8266_BIN0.92.bin...
-- verify OK (digest matched)

8.firmware actualizado





