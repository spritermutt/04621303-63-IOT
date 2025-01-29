#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

IRAM_ATTR void handleInterrupt();
#define WIFI_STA_NAME "piper"          // ชื่อ wifi
#define WIFI_STA_PASS "Wifionly"   // รหัส wifi
#define MQTT_SERVER "broker.emqx.io" // Server Domain Name หรือ IP Address
#define MQTT_PORT 1883               // Port MQTT Broker
#define MQTT_USERNAME "Realpeach"
#define MQTT_PASSWORD "1212312121"
#define MQTT_NAME "RxxlPxxch"  // ชื่อที่ต้องการให้แสดงใน MQTT Broker
#define MQTT_TOPIC "Led_Control" // ชื่อ Topic

WiFiClient client;
PubSubClient MQTT(client);

const int CONNECT_CHECK = 0;
const int SUB_MESSAGE = 1;
const int DISPLAY_MESSAGE = 2;
int value = 0;
int flag = false;
bool message_flag;
String message;
int state;

void callback(char *topic, byte *payload, unsigned int length) 
{
  message = "";  
  message_flag = true; 
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i]; 
  }
}

void setup()
{
  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT);
  attachInterrupt(digitalPinToInterrupt(D2), handleInterrupt, RISING);
  Serial.begin(115200);
  WiFiManager wm;
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("MyWiFi"); // anonymous ap
  // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
  }
  else
  {
    Serial.println("connected...yeey :)");
  }

  state = CONNECT_CHECK;

  MQTT.setServer(MQTT_SERVER, MQTT_PORT);
  MQTT.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD);
  MQTT.setCallback(callback);                            
  MQTT.subscribe(MQTT_TOPIC);
}
void loop()
{
  switch (state)
  {
  case CONNECT_CHECK:
    if (MQTT.connected())
    {
      Serial.println("MQTT Connected.");
      state = SUB_MESSAGE;
    }
    else
    {
      Serial.println("MQTT Fail Connected.");
    }
    break;

  case SUB_MESSAGE:
    MQTT.loop();
    if (flag == true)
    {
      flag = false;
      delay(50);
      if (digitalRead(D2) == 1)
      {
        if (value == 1)
        {
          digitalWrite(D1, 1);
          MQTT.publish(MQTT_TOPIC, "LED ON");
          value = 0;
        }
        else if (value == 0)
        {
          digitalWrite(D1, 0);
          MQTT.publish(MQTT_TOPIC, "LED OFF");
          value = 1;
        }
      }
    }

    if (message_flag == true)
    {
      state = DISPLAY_MESSAGE;
    }
    break;

  case DISPLAY_MESSAGE:
    Serial.println(message);
    String msg = message;
    if (msg == "on")
    {
      digitalWrite(D1, HIGH);
      value = 0;
    }
    else if (msg == "off")
    {
      digitalWrite(D1, LOW);
      value = 1;
    }
    message_flag = false;
    state = SUB_MESSAGE;
    break;
  }
}

IRAM_ATTR void handleInterrupt()
{
  flag = true;
}
