#include<ESP8266WiFi.h>
#include<Arduino.h>

#define SSID "TP-Link_A287"
#define PASSWORD "66845355"
void initWiFi();

void setup() {
  Serial.begin(115200);
  initWiFi();
}
void loop(){
  if (WiFi.status() == WL_CONNECTED)
  {
    delay(3000);
    Serial.println("Connected to the WiFi network");
      Serial.println(WiFi.localIP());
  }else{
    Serial.println("Not connected to the WiFi network");
    initWiFi();
  }
  
}

void initWiFi() {
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


