#include <WiFiManager.h>
#include <Arduino.h>


void setup() {
    Serial.begin(115200);
    WiFiManager wm;
    wm.resetSettings();
    bool res;
    res = wm.autoConnect("group1","password");

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        Serial.println("connected...yeey :)");
    }

}

void loop() {
}