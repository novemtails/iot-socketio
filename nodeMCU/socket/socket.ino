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

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);
ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

void event(const char * payload, size_t length) {
  USE_SERIAL.printf("got message: %s\n ZZZZZZ", payload);
}

int led = 2;
bool ledOnStatus;

void setup() {
  // Display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome");
  display.display();
  
  pinMode(led, OUTPUT);
    USE_SERIAL.begin(115200);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP("SSID", "passpasspass");

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.on("event", response);
    webSocket.begin("192.168.0.11", 3000);
    webSocket.emit("Connection", "\"Hola, soy nodeMCU\"");
    
    webSocket.on("connect", response);
    webSocket.on("ledOn", ledOn);
    
    
    // use HTTP Basic Authorization this is optional remove if not needed
    //webSocket.setAuthorization("username", "password");
}

void response(const char * response, size_t length) {
          USE_SERIAL.println(response);
          display.println(response);
        }

void ledOn(const char * ledOn, size_t length){
  USE_SERIAL.println("Led Encendido");
  USE_SERIAL.println(ledOn);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);  
  display.println(ledOn);
  display.display();
  ledOnStatus = true;
  }
  void ledOff(const char * ledOff, size_t length){
    USE_SERIAL.println("Led Apagado");
  ledOnStatus = false;
  }

void loop() {
  display.clearDisplay();
    webSocket.loop();
    webSocket.on("ledOn", ledOn);
    if(ledOnStatus){
      digitalWrite(led, HIGH);
      display.println("Led Encendido");
      }
    else{
      digitalWrite(led, LOW);
      }
}
