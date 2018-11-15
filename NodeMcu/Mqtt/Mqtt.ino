#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "KKNhome";
const char* password = "mdk1208Ntech";

byte server[] = {192, 168, 0, 14};
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "sensors/test/temperature";

WiFiClient espClient;
PubSubClient client(server,1883,callback,espClient);

void setup()
{
    Serial.begin(115200);
    setupWifi();
    client.connect("ESP8266Client", "hello", 1, true, "unexpected Disconnect");
    client.subscribe("firstTopic");
}
void loop()
{
    if (!client.connected()) {
    reconnect();
    client.subscribe("firstTopic");
    }
    client.loop();
}


void setupWifi() {
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", "hello", 1, true, "unexpected Disconnect")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

