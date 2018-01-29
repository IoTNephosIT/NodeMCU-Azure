
/*
 Basic ESP8266 MQTT example

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

 Based on the sample located here https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
 Modified to connect to Azure IoTHub
 
 You must change the default maximun package size in the PubSubClient.h file to 256 like this 
  #ifndef MQTT_MAX_PACKET_SIZE
    #define MQTT_MAX_PACKET_SIZE 256
  #endif
*/
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <WiFiClientSecure.h>

const char* ssid = "MyNetworkName";
const char* password = "MyNetworkPassword";
// your IoTHub URL, it is <your iot hub name>.azure-devices.net
const char* mqtt_server = "testesp123.azure-devices.net";

// Default WifiClient must be change to WiFiClientSecure
WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
	// Use the option that recives "DeviceID, user, password"
	// In order to generate the password you have to create a SAS, the easier wat to do that is using IotHub Device Explorer https://github.com/Azure/azure-iot-sdk-csharp/tree/master/tools/DeviceExplorer
    if (client.connect("TestDevice", "testesp123.azure-devices.net/TestDevice", "SharedAccessSignature sr=testesp123.azure-devices.net&sig=gBFdLY%2F0X2Ntyk9hKm7ChCtQISEKErSAYhWibaLWCZk%3D&se=1548716725&skn=iothubowner")) {
      Serial.println("connected");
      
	  // change the in and out topic, just replace the device name bellow this line
      client.publish("devices/TestDevice/messages/events/", "hello world");
      client.subscribe("devices/TestDevice/messages/devicebound/#");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 8883); // The port used by Azure is 8883, that must be changed
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
