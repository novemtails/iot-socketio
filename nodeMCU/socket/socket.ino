#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <SocketIoClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

void event(const char * payload, size_t length) {
  USE_SERIAL.printf("got message: %s\n", payload);
}

int led = 2;
bool ledOnStatus;

void setup() {

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
    
    
    // use HTTP Basic Authorization this is optional remove if not needed
    //webSocket.setAuthorization("username", "password");
}

void response(const char * response, size_t length) {
          USE_SERIAL.println(response);
        }

void ledOn(const char * ledOn, size_t length){
  USE_SERIAL.println("Led Encendido");
  ledOnStatus = true;
  }
  void ledOff(const char * ledOff, size_t length){
    USE_SERIAL.println("Led Apagado");
  ledOnStatus = false;
  }
void loop() {
    webSocket.loop();
    webSocket.on("ledOn", ledOn);
    webSocket.on("ledOff", ledOff);

    if(ledOnStatus){
      digitalWrite(led, HIGH);
      }
    else{
      digitalWrite(led, LOW);
      }
}
