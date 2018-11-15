#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
String myString;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED};

byte server[] = {192, 168, 1, 14};
byte ip[] = {192, 168, 1, 34};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 1, 1};

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  Serial.println("Ethernet Begin");
}
void loop()
{
  //client.connect(mqttClientId, (char*)mqttUser, (char*)mqttPass, lastWillTopic, 0, true, "Client lost connection");
  //client.connect("Arduino1","lastWillTopic", 1, true, "Client lost connection");
  while (!client.connected()) {
    Serial.println("Trying to connect client to server...");
    // client.connect("My_arduinoClient");
    client.connect("Arduino1", "lastWillTopic", 1, true, "Client lost connection");
    if (client.connected()) {
      Serial.println("Client Connected!!");
    }
  }
  if (client.connected()) {
    Serial.println("Sent Data to Broker");
    client.publish("outTopic", "Hello,How are you!");
    client.subscribe("inTopic");
    delay(5000);
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("myString:");
  Serial.println(myString);
  Serial.println();
}


