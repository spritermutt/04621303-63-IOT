#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_USERNAME "Realpeach"
#define MQTT_PASSWORD "1212312121"
#define MQTT_NAME "RxxlPxxch"
#define MQTT_TOPIC "helloPeaches"
#define RESPONSE_TOPIC "helloPeachesResponse"

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);

bool message_flag = false;
String message;
unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 5000; // Publish every 5 seconds

enum State { CONNECT_CHECK, SUB_MESSAGE, DISPLAY_MESSAGE };
State state = CONNECT_CHECK;

void callback(char *topic, byte *payload, unsigned int length) {
  String incomingMessage = "";
  for (unsigned int i = 0; i < length; i++) {
    incomingMessage += (char)payload[i];
  }
  // Check if the message is from the response topic
  if (strcmp(topic, RESPONSE_TOPIC) == 0) {
    message = incomingMessage;
    message_flag = true; // Set the flag to indicate a valid message
  }
}

void setup() {
  Serial.begin(115200);
  WiFiManager wm;
  bool res = wm.autoConnect("MyWiFi");
  if (!res) {
    Serial.println("Failed to connect to Wi-Fi");
    return;
  }
  Serial.println("Connected to Wi-Fi");

  MQTT.setServer(MQTT_SERVER, MQTT_PORT);
  MQTT.setCallback(callback);

  while (!MQTT.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT Connected.");
  MQTT.subscribe(RESPONSE_TOPIC); // Subscribe to the response topic
}

void loop() {
  switch (state) {
    case CONNECT_CHECK:
      if (MQTT.connected()) {
        Serial.println("MQTT Connected.");
        state = SUB_MESSAGE;
      } else {
        Serial.println("MQTT Disconnected. Attempting reconnection...");
        if (MQTT.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
          Serial.println("MQTT Reconnected.");
          MQTT.subscribe(RESPONSE_TOPIC); // Resubscribe after reconnection
        }
      }
      break;

    case SUB_MESSAGE:
      MQTT.loop();
      if (millis() - lastPublishTime >= publishInterval) {
        MQTT.publish(MQTT_TOPIC, "Chusanapak Fongmanee"); // Publish to Node-RED
        Serial.println("Published: Chusanapak Fongmanee");
        lastPublishTime = millis();
        state = DISPLAY_MESSAGE;
      }
      break;

    case DISPLAY_MESSAGE:
      if (message_flag) {
        Serial.println("Response from Node-RED: " + message);
        message_flag = false; // Reset the flag after processing
      }
      state = SUB_MESSAGE;
      break;
  }
}