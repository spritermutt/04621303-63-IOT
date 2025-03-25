#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <ArduinoJson.h> // ใช้ไลบรารี ArduinoJson เพื่อจัดการกับ JSON


#define MQTT_SERVER "broker.emqx.io" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883 // Port ของ MQTT Broker
#define MQTT_TOPIC_INFO "Group1RealpeachInfo" // ชื่อ Topic ส่งข้อมูลส่วนตัว
#define MQTT_TOPIC_HELLO "Group1RealpeachHello" // ชื่อ Topic รับข้อความ hello
#define MQTT_NAME "rmutt"
#define LED_PIN D3 // กำหนดขาที่ใช้เป็น LED

WiFiClient client;
PubSubClient mqtt(client);

const int SEND_AND_CHECK = 0;
const int SHOW_MESSAGE = 1;
int state = SEND_AND_CHECK;

JsonDocument doc_pub, doc_sub;
char jsonBuffer[512];
bool message_flag = false;
String message = "";
float temperature_sensor; /////////////////////////////////////////////////////////

void reconnectMQTT();

void callback(char *topic, byte *payload, unsigned int length) {
  message = ""; // Clear the message buffer
  message_flag = true; // Set flag to indicate new data
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // Append received data to message
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFiManager wm;
  bool res = wm.autoConnect("WIFI"); // Anonymous AP
  if (!res) {
    Serial.println("Failed to connect, restarting...");
    ESP.restart();
  } else {
    Serial.println("Connected to WiFi!");
  }

  pinMode(LED_PIN, OUTPUT);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
  
  reconnectMQTT(); // Ensure MQTT connection before proceeding
}

void loop() {
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop(); // Keep MQTT alive
  int sensorValue = analogRead(A0); /////////////////////////////////////////////////////////
  int PWM = map(sensorValue, 0, 1023, 0, 100); /////////////////////////////////////////////////////////
  switch (state) {
    case SEND_AND_CHECK:
      Serial.print("Humandity: "); /////////////////////////////////////////////////////////
      Serial.println(PWM); /////////////////////////////////////////////////////////

      if (mqtt.connected()) {
        doc_pub.clear(); // Clear JSON buffer before use
        doc_pub["humandity"] = PWM;
        serializeJson(doc_pub, jsonBuffer);
        
        if (mqtt.publish(MQTT_TOPIC_INFO, jsonBuffer)) {
          Serial.println("Success sending JSON");
        } else {
          Serial.println("Fail sending JSON");
        }
      } else {
        Serial.println("MQTT not connected, retrying...");
      }

      if (message_flag) {
        state = SHOW_MESSAGE;
      }
      delay(2000); // Delay before next cycle
      break;

    case SHOW_MESSAGE:
      deserializeJson(doc_sub, message);
      if (doc_sub.containsKey("led")) {
        const char *led_state = doc_sub["led"];
        Serial.print("led state: ");
        Serial.println(led_state);
        if (strcmp(led_state, "open") == 0) {
          digitalWrite(LED_PIN, HIGH);
          Serial.println("led is open. LED turned on.");
        } else {
          digitalWrite(LED_PIN, LOW);
          Serial.println("led is closed. LED turned off.");
        }
      } else {
        Serial.println("No 'led' key found in received JSON");
      }
      state = SEND_AND_CHECK;
      break;
  }
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    delay(10000);
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_NAME)) {
      Serial.println("Connected to MQTT");
      mqtt.subscribe(MQTT_TOPIC_HELLO); // Subscribe after connecting
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}
