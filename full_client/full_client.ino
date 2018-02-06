#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "PubSubClient.h"
#include "DHT.h"
#include "Config.h"
#define DHTTYPE DHT11

const int DHTPin = 2;
const char* ssid = NETWORK_SSID;
const char* password = NETWORK_PASSWORD;
const char* mqtt_server = MQTT_URL;

DHT dht(DHTPin, DHTTYPE);
WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection to Azure IoT Hub...");

    if (client.connect(DEVICE_NAME, USER_NAME, SAS)) {
      Serial.println("connected");
      client.subscribe(INPUT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    String payload = "{";
    payload += "\"t\":\"";
    payload += t;
    payload += "\",\"h\":\"";
    payload += h;
    payload += "\"}";


    Serial.print("Publish message: ");
    Serial.println(payload);
    client.publish(OUTPUT_TOPIC, (char*) payload.c_str());
  }
}
