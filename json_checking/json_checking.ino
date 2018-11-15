#include <ArduinoJson.h>
#include <string.h>
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
String topicCallback;
boolean isJson = false;
boolean messageRecieved = false;
char messageBuff[400];
char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

byte mac[] = {0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED};

// LocalServer with MQTT Broker[ Mosquitto]
byte localServer[]={192,168,3,2};

//My Arduino's IP address
byte ip[]={192,168,3,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,3,1};

//Initializing the Client 
EthernetClient ethClient;

//Creating the PubSubClient Object
PubSubClient client(localServer,1883,callback,ethClient);

// RFID Pin Selection
#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
    {
    Serial.begin(9600);
    Ethernet.begin(mac,ip,gateway,subnet);
    Serial.println("Ethernet Initialized :)");
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized :)"));
    while(!client.connected()){
        Serial.println("Trying to connect client to server...");
        client.connect("Arduino1","RU_<DE,AD,BE,FF,06,ED>", 1, true, "Unexpected Disconnect");
        if(client.connected()){
            Serial.println ("Success!!, Client Connected! ");
        }
    }
    client.subscribe("JSON");
    client.subscribe("JSONh");
}

void loop(){
    mfrc522.PICC_HaltA();
    if( messageRecieved == true ){
        messageRecieved = false;
        validateJson("JSON");
      } 
    if( messageRecieved == true ){
        messageRecieved = false;
        validateJson("JSONh");
    } 
    client.loop(); 
}
void callback(char* topic,byte* payload,unsigned int length)
{ 
    topicCallback = topic;
    Serial.print("Topic Arrived :[");
    Serial.print(topic);
    Serial.println("]");
    int i;
    for(i=0;i<length;i++) {
        messageBuff[i]=payload[i];
    }
    messageBuff[i]='\0';
    Serial.println("Message Arrived :");
    Serial.println(String(messageBuff));
    messageRecieved = true;
    StaticJsonBuffer<400> jsonBuffer;
    if( topicCallback == "JSON") {
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
        checkJsonFlag(root);
    } else if( topicCallback == "JSONh") {
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
         checkJsonFlag(root);
      }  
}

void validateJson( String msg ) {
     if( topicCallback == msg) {
            if(isJson) {
                Serial.println("Json recieved");
                isJson = false;
            } else{
                Serial.println("Msg is not a Json");
            }
      }
}

void checkJsonFlag( JsonObject& root) {
     if (!root.success()) {
            Serial.println("JsonNotValid");
            isJson = false;
     } else {
            Serial.println("Json found");
            isJson = true;
     }
}

