#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <ArduinoJson.h> // ใช้ไลบรารี ArduinoJson เพื่อจัดการกับ JSON
#include <SPI.h>
#include <MFRC522.h>

#define MQTT_SERVER "broker.emqx.io" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883 // Port ของ MQTT Broker
#define MQTT_TOPIC_INFO "Group1RealpeachInfo" // ชื่อ Topic ส่งข้อมูลส่วนตัว
#define MQTT_TOPIC_HELLO "Group1RealpeachHello" // ชื่อ Topic รับข้อความ hello
#define MQTT_NAME "rmutt"
#define RST_PIN D3
#define SS_PIN D8
#define LED_PIN D2

MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid_in = ""; 
String dump_byte_array(byte *buffer, byte bufferSize);

WiFiClient client;
PubSubClient mqtt(client);

const int SEND_AND_CHECK = 0;
const int SHOW_MESSAGE = 1;
int state;

JsonDocument doc_pub, doc_sub;
char jsonBuffer[512];
bool message_flag = false;
String message = "";

void reconnectMQTT();
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
  res = wm.autoConnect("Group1RealPeach"); // anonymous ap
  if(!res) {
    Serial.println("Failed to connect");
    ESP.restart(); // Restart if WiFi connection fails
  } else {
    Serial.println("connected...yeey :)");
  }

  pinMode(LED_PIN, OUTPUT);

  state = SEND_AND_CHECK;
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
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
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        rfid_in = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println("RFID detected: " + rfid_in);

        if (mqtt.connected()) {
          doc_pub["name"] = rfid_in;
          serializeJson(doc_pub, jsonBuffer);
          if (mqtt.publish(MQTT_TOPIC_INFO, jsonBuffer)) {
            Serial.println("Success sending JSON");
          } else {
            Serial.println("Fail sending JSON");
          }
          rfid_in = ""; // Clear RFID input after publishing
        } else {
          Serial.println("MQTT not connected, trying to reconnect...");
        }
      }

      if (message_flag) {
        state = SHOW_MESSAGE;
      }
      delay(2000);
      break;

    case SHOW_MESSAGE:
  deserializeJson(doc_sub, message);
  if (doc_sub.containsKey("door")) {
    const char* door_state = doc_sub["door"];
    Serial.println("Door state: " + String(door_state));

    // Check if the door should be opened
    if (strcmp(door_state, "open") == 0) {
      digitalWrite(LED_PIN, HIGH); // Turn on the LED
      Serial.println("Door is open. LED turned on.");
      delay(3000); // Wait for 5 seconds
      digitalWrite(LED_PIN, LOW); // Turn off the LED
    } else {
      digitalWrite(LED_PIN, LOW); // Turn off the LED
      Serial.println("Door is closed. LED turned off.");
    }
  } else {
    Serial.println("No 'door' key found in received JSON");
  }
  message_flag = false;
  state = SEND_AND_CHECK;
  break;}
}
String dump_byte_array(byte *buffer, byte bufferSize) {
  String content = "";
  for (byte i = 0; i < bufferSize; i++) {
    content.concat(String(buffer[i] < 0x10 ? " 0" : " "));
    content.concat(String(buffer[i], HEX));
  }
  content.toUpperCase();
  return content;
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