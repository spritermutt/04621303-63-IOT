#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <ArduinoJson.h> // ใช้ไลบรารี ArduinoJson เพื่อจัดการกับ JSON

#define MQTT_SERVER "broker.emqx.io" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883 // Port ของ MQTT Broker
#define MQTT_TOPIC_INFO "Group1RealpeachInfo" // ชื่อ Topic ส่งข้อมูลส่วนตัว
#define MQTT_TOPIC_HELLO "Group1RealpeachHello" // ชื่อ Topic รับข้อความ hello
#define MQTT_NAME "rmutt"

WiFiClient client;
PubSubClient mqtt(client);
const int SEND_AND_CHECK = 0;
const int SHOW_MESSAGE = 1;
int state;
JsonDocument doc_pub, doc_sub;
char jsonBuffer[512];
bool message_flag;
String message;

void callback(char *topic, byte *payload, unsigned int length) {
  message = ""; // ล้างข้อมูลที่เก็บไว้ในตัวแปร message
  message_flag = true; // กำหนดค่า message_flag เป็น true เพื่อบอกให้รู้ว่ามีข้อมูลใหม่เข้ามา
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // เก็บข้อมูลที่ได้รับไว้ในตัวแปร message
  }
}
void reconnectMQTT()
void setup() {
  Serial.begin(115200);
  
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("Group1RPAP"); // anonymous ap

  if(!res) {
    Serial.println("Failed to connect");
  } else {
    Serial.println("connected...yeey :)");
  }

  state = SEND_AND_CHECK;

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.connect(MQTT_NAME);
  mqtt.setCallback(callback);
  mqtt.subscribe(MQTT_TOPIC_HELLO);
}
void loop() {
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop();

  switch (state) {
    case SEND_AND_CHECK:
      // Check if we're still connected to the broker
      if (mqtt.connected()) {
        // Create JSON object and publish message
        doc_pub["name"] = "Chusanapak Fongmanee";
        doc_pub["address"]["city"] = "Nakhon Si Thammarat";
        doc_pub["address"]["postcode"] = "80290";
        serializeJson(doc_pub, jsonBuffer);

        if (mqtt.publish(MQTT_TOPIC_INFO, jsonBuffer) == true) {
          Serial.println("Success sending JSON");
        } else {
          Serial.println("Fail sending JSON");
        }

        if (message_flag) {
          state = SHOW_MESSAGE;
        }
      } else {
        // If not connected, try to reconnect
        Serial.println("MQTT not connected, trying to reconnect...");
      }
      delay(2000);
      break;

    case SHOW_MESSAGE:
      deserializeJson(doc_sub, message);
      const char* doc_sub_message = doc_sub["message"];
      Serial.println(doc_sub_message);
      message_flag = false;
      state = SEND_AND_CHECK;
      break;
  }
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_NAME)) {
      Serial.println("connected");
      mqtt.subscribe(MQTT_TOPIC_HELLO);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}
