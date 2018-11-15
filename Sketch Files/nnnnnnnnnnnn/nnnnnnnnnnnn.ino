#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
String myString;

byte mac[] = {0x11, 0x11, 0x11, 0x00, 0x00, 0x01};

byte server[] = {192, 168, 3, 14};
byte ip[] = {192, 168, 3, 34};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 3, 1};

String clientId;
EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

String myMacId = "11-11-11-00-00-01";
String clientId = "RU_" + myMacId;
String clientState = "client/state/RU_" + myMacId;
String getRuDetail = "GET_RU_DETAIL";
String setRuDetail = "SET_RU_DETAIL_" + myMacId;
String rfidSubscribeStr;
String ruidStr;
String errorJson;
String getPayload;
String topicCallback;

//char Buffers
char clientIdBuff[50];
char clientStateBuff[50];
char getRuDetailBuff[50];
char setRuDetailBuff[50];
char myMacIdBuff[50];
char rfidSubscribeStrBuff[50];
char messageBuff[450];
char rfidDetailBuff[50];
char rfidPublishStrBuff[50];
char msgInfoBuff[100];

//Boolean
boolean getRuDetailFlag = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Ethernet Begin");
  Ethernet.begin(mac, ip, gateway, subnet);
}

void loop()
{
    if (!client.connected()) {
        Serial.println("Trying to connect to MQTT Broker...");
        client.connect(clientId, clientId, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println ("Success!! MQTT:" + myMacId + " Connected! ");
            clientId.toCharArray(msgInfoBuff, 100);
            client.publish("client/state/" + clientId, msgInfoBuff, true); //also send retainflag = true
            Serial.println("Subscribe to " + setRuDetail);
            client.subscribe(setRuDetail);
        } else {
            myDelay(10);
        }
        getRuDetailFlag = false;
    } else {
        client.loop();
    }
}

void myDelay(unsigned long sec)
{
    unsigned long delayMSec = sec * 1000;
    unsigned long recordMSec = millis();
    while((millis() - recordMSec) < delayMSec);
}

void callback(char* topic, byte* payload, unsigned int length) {
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


