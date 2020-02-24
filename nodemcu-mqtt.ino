// DO NOT FORGET TO CHANGE WIFI SSID/PASSWORD
// CHECK YOUR MQTT BROKER CREDENTIALS
// BESURE THE PINS ARE CONNECTED CORRECTLY

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "sabaiheart";
const char* password = "ok0boVu1";

// Config MQTT Server
#define mqtt_server "m12.cloudmqtt.com"
#define mqtt_port 18109
#define mqtt_user "vplfvxvo"
#define mqtt_password "g9ssGd-XOM4z"

#define AIR_PIN 4
#define FOG_PIN 5

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(AIR_PIN, OUTPUT);
  
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("/SWF");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  int i=0;
  while (i<length) msg += (char)payload[i++];
  if (msg == "GET_FAN") {
    client.publish("/SWF", (digitalRead(AIR_PIN) ? "FANON" : "FANOFF"));
    Serial.println("Send !");
    return;
  }
  
  if (msg == "AIR_ON") {
    digitalWrite(AIR_PIN, HIGH);
  }
  if (msg == "AIR_OFF") {
    digitalWrite(AIR_PIN, LOW);
  }
  if (msg == "FOG_ON") {
    digitalWrite(FOG_PIN, HIGH);
  }
  if (msg == "FOG_OFF") {
    digitalWrite(FOG_PIN, LOW);
  }
  
  Serial.println(msg);
}
