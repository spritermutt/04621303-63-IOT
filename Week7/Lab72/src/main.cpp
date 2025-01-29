#include <Arduino.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// MQTT Configuration
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_TOPIC_INFO "Group1RealPeachInfo"
#define MQTT_TOPIC_HELLO "Group1RealPeachHello"
#define MQTT_NAME "emqx"

// State definitions
const int SEND_AND_CHECK = 0;
const int SHOW_MESSAGE = 1;

// Global variables
WiFiClient client;
PubSubClient mqtt(client);
int state;
StaticJsonDocument<200> doc_pub;
StaticJsonDocument<200> doc_sub;
char jsonBuffer[512];
bool message_flag = false;
String message;

// Enhanced MQTT callback for Node-RED messages
void callback(char *topic, byte *payload, unsigned int length) {
  // Clear existing message and set flag
  message_flag = true;
  message = "";
  
  // Print raw payload for debugging
  Serial.println("\n----- New MQTT Message -----");
  Serial.print("Raw payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  // Convert payload to string
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // Print detailed debug info
  Serial.println("Topic: " + String(topic));
  Serial.println("Message length: " + String(length));
  Serial.println("Decoded message: " + message);
  Serial.println("--------------------------\n");

  // Try to parse JSON if the message is in JSON format
  DeserializationError error = deserializeJson(doc_sub, message);
  if (!error) {
    Serial.println("JSON parsed successfully");
    serializeJsonPretty(doc_sub, Serial);
    Serial.println();
  }
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_NAME)) {
      Serial.println("connected");
      
      // Subscribe and verify subscription
      boolean subSuccess = mqtt.subscribe(MQTT_TOPIC_HELLO);
      Serial.print("Subscription to ");
      Serial.print(MQTT_TOPIC_HELLO);
      Serial.println(subSuccess ? " successful" : " failed");
      
      // Print current MQTT state
      Serial.print("MQTT State: ");
      Serial.println(mqtt.state());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // Start Serial with higher baud rate
  Serial.begin(115200);
  Serial.println("\n\n----- Starting Setup -----");
  
  // WiFiManager setup
  WiFiManager wm;
  wm.resetSettings();
  bool res = wm.autoConnect("Group1", "wifionly");
  
  if (!res) {
    Serial.println("Failed to connect to WiFi");
    ESP.restart();
  } else {
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  
  // MQTT setup
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
  
  state = SEND_AND_CHECK;
  Serial.println("----- Setup Complete -----\n");
}

void loop() {
  switch (state) {
    case SEND_AND_CHECK:
      if (!mqtt.connected()) {
        reconnect();
      }
      
      mqtt.loop();
      
      // Publish status message
      doc_pub.clear();
      doc_pub["firstname"] = "Chusanapak";
      doc_pub["lastname"] = "Fongmanee";
      doc_pub["address"]["city"] = "Bangkok";
      doc_pub["address"]["country"] = "Thailand";
      serializeJson(doc_pub, jsonBuffer);
      
      if (mqtt.publish(MQTT_TOPIC_INFO, jsonBuffer)) {
        Serial.println("Published message:");
        serializeJsonPretty(doc_pub, Serial);
        Serial.println();
                state = SHOW_MESSAGE;

      }
      
      if (message_flag) {
      }
      delay(5000);
      break;

    case SHOW_MESSAGE:
      Serial.println("\n----- Displaying Received Message -----");
      Serial.println("Message from Node-RED: " + message);
      Serial.println("-------------------------------------\n");
      message_flag = false;
      state = SEND_AND_CHECK;
      delay(5000);
      break;
  }
}