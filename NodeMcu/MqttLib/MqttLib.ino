#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>

//const char* ssid = "KKNhome";
const char* ssid = "CPSgateway-wifi";
const char* password = "DhashanTech";

//byte server[] = {192, 168, 0, 14};
byte server[] = {192, 168, 3, 8};
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "sensors/test/temperature";

String topicCallback;
boolean messageReceived = false;

WiFiClient espClient;
PubSubClient client(server,1883,callback,espClient);

void setup()
{
    Serial.begin(115200);
    setupWifi();
    client.connect("ESP8266Client", "hello", 1, true, "unexpected Disconnect");
    subscribe("firstTopic");
}
void loop()
{
    if (!client.connected()) {
    reconnect();
    subscribe("firstTopic");
    }
    publish("NodeMcu","Hello,World!",false);
    myDelay(2);
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
  messageReceived = true;
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


void publish(String topic, String msg, boolean retainFlag)
{
    char topicBuf[100];
    char msgBuf[400];
    Serial.println("Publish: Topic = " + topic + " Msg = " + msg);
    topic.toCharArray(topicBuf, topic.length() + 1);
    msg.toCharArray(msgBuf, msg.length()+1);
    client.publish(topicBuf, msgBuf, retainFlag);
}

void subscribe(String topic) {
    char topicBuf[100];
    Serial.println("Subscribe to " + topic);
    topic.toCharArray(topicBuf, topic.length() + 1);
    client.subscribe(topicBuf);   
}

void unsubscribe(String topic) {
    char topicBuf[100];
    Serial.println("unsubscribe to " + topic);
    topic.toCharArray(topicBuf, topic.length() + 1);
    client.unsubscribe(topicBuf);   
}

void myDelay(unsigned long sec)
{
    unsigned long delayMSec = sec * 1000;
    unsigned long recordMSec = millis();
    messageReceived = false;
    //while loop will run untill intervel time,then it will comeout
    while(((millis() - recordMSec) < delayMSec) && (!messageReceived)) {
        client.loop();
    }
}

