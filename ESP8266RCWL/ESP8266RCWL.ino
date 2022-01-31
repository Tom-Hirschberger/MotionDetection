#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "settings.h"
 
WiFiClient espClient;
long interval = 10;
bool inputFlag = false;

void ICACHE_RAM_ATTR inputDetected() {
  inputFlag = true;
}
 
void setup() {
    Serial.begin(115200);
    setupWifi();
    pinMode(digitalPinToInterrupt(gpioUsed), INPUT);
    attachInterrupt(digitalPinToInterrupt(gpioUsed), inputDetected, RISING);
}
 
void setupWifi() {;
    WiFi.mode(WIFI_STA);

    WiFi.begin(SSID, PSK);

    while (WiFi.status() == WL_DISCONNECTED) {
      delay(500);
    }

    if (WiFi.status() != WL_CONNECTED) {
      delay(10);
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(SSID);
   
      WiFi.begin(SSID, PSK);
   
      while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if(WiFi.status()== WL_CONNECTED){
      if (inputFlag){
        inputFlag = false;
        Serial.println("Input detected!");
        HTTPClient http;
        http.begin(espClient, serverUrl.c_str());
        // Send HTTP GET request
        int httpResponseCode = http.GET();
      
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
        }

        http.end();

        delay(delayAfterInput);
        inputFlag = false;
      }
    } else {
      Serial.println("Wifi not connected. Reconnect!");
      setupWifi();
    }
    delay(interval);
}
