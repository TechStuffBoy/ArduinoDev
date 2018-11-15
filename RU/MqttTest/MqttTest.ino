#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>

byte mac[] = {0x11, 0x11, 0x11, 0x00, 0x00, 0x01};

byte server[] = {192, 168, 3, 14};
byte ip[] = {192, 168, 3, 34};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 3, 1};


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
char messageBuff[400];

//Boolean
boolean messageReceived = false;
boolean getRuDetailFlag = false;

void setup()
{
    Serial.begin(9600);
    Serial.println("Ethernet Begin");
    Ethernet.begin(mac, ip, gateway, subnet);
    clientId.toCharArray(clientIdBuff, (clientId.length() + 1));
}

void loop()
{
    if (!client.connected()) {
        Serial.println("Trying to connect to MQTT Broker...");
        client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println ("Success!! MQTT:" + myMacId + " Connected! ");
            publish(clientState, clientId, true); //also send retainflag = true
            Serial.println("Subscribe to " + setRuDetail);
            subscribe(setRuDetail);
        } else {
            delay(10000);
        }
        getRuDetailFlag = false;
    } else {
        Serial.println("Client Connected");
        client.loop();
        myDelay(10);
    }
}

void publish(String topic, String msg, boolean retainFlag)
{
    char topicBuf[100];
    char msgBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    msg.toCharArray(msgBuf, msg.length()+1);
    client.publish(topicBuf, msgBuf, retainFlag);
}

void subscribe(String topic) {
    char topicBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    client.subscribe(topicBuf);   
}

void myDelay(unsigned long sec)
{
    unsigned long delayMSec = sec * 1000;
    unsigned long recordMSec = millis();
    messageReceived = false;
    while(((millis() - recordMSec) < delayMSec) && (!messageReceived)) {
        client.loop();
    }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
    int i=0;
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.println("] ");
    
    for( i=0;i<length;i++){
        messageBuff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    messageBuff[i]='\0';
    Serial.print("messageBuff:");
    Serial.println(messageBuff);
    Serial.println();
    messageReceived = true;
}


