#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SocketIoClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define USE_SERIAL Serial
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR   0x3C
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);
ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

void setup() {
  while(WiFiMulti.run() != WL_CONNECTED) {
        delay(5000);
        WiFiMulti.addAP("BSSID", "password");// nombre y contrasena de nuestra red WIFI
    }
  // Display
  WiFi.mode(WIFI_STA);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome");// mensaje de bienvenida en nuestra pantalla
  display.display();
  
    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }
    
    webSocket.begin("192.168.0.5", 3000);// direccion del servidor en nuestra red local
    webSocket.emit("Connection", "\"Hi, I'm nodeMCU\"");
    
    webSocket.on("connect", response);
    webSocket.on("message", message);// Escucha el encabezado message que se envia desde el servidor
    }

void response(const char * response, size_t length) {
          USE_SERIAL.println(response);
          display.println(response);
        }

//Funcion que se ejecuta cuando se recibe un nuevo mensaje desde el servidor
void message(const char * message, size_t length){
  USE_SERIAL.println(message);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);  
  display.println(message);// imprime el mensaje recibido en nuestra pantalla OLED
  display.display();
}

void loop() {  
if(WiFi.status() == WL_CONNECTED) {
    webSocket.loop();
  } else {
    webSocket.disconnect();
  }        
}
