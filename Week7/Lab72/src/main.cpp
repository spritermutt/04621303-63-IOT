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

bool message_flag;
String message;

void callback(char *topic, byte *payload, unsigned int length) {
  message = ""; // ล้างข้อมูลที่เก็บไว้ในตัวแปร message
  message_flag = true; // กำหนดค่า message_flag เป็น true เพื่อบอกให้รู้ว่ามีข้อมูลใหม่เข้ามา
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // เก็บข้อมูลที่ได้รับไว้ในตัวแปร message
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFiManager wm;
  bool res;
  wm.resetSettings(); // ลบข้อมูลการเชื่อมต่อ WiFi ที่เคยบันทึกไว้
  res = wm.autoConnect("Group1RealPeach"); // anonymous ap

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
  switch (state) {
    case SEND_AND_CHECK:
      if (mqtt.connected()) {
        mqtt.loop();
        
        // สร้าง JSON object
        StaticJsonDocument<512> jsonDoc;
        jsonDoc["name"] = "Chusanapak Fongmanee"; // ชื่อนักศึกษา
        jsonDoc["address"]["city"] = "Bangkok"; // ชื่อเมือง
        jsonDoc["address"]["postcode"] = "10000";


        char jsonBuffer[200];
        serializeJson(jsonDoc, jsonBuffer);

        if (mqtt.publish(MQTT_TOPIC_INFO, jsonBuffer)) {
          Serial.println("Success sending JSON");
        } else {
          Serial.println("Fail sending JSON");
        }

        if (message_flag) {
          state = SHOW_MESSAGE;
        }
      }
      delay(5000);
      break;

    case SHOW_MESSAGE:
      Serial.println("Received message: " + message);
      message_flag = false;
      state = SEND_AND_CHECK;
      break;
  }
}