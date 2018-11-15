// Library For Ethernet Shield
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>
int i=0;
                 
//Arduino Mac Address
byte mac[] ={0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED};
String myMacId = "DE,AD,BE,FF,06,ED";

// LocalServer with MQTT Broker[ Mosquitto]
byte localServer[]={192,168,3,14};

//My arduino's Ip Details
byte ip[]={192,168,3,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,3,1};

//Initializing the Client 
EthernetClient ethClient;

//Creating the PubSubClient Object
PubSubClient client(localServer,1883,callback,ethClient);

                /****Ethernet and MQTT Declr****/
                
//Buffers For Storing Values
char message_buff[400];  //for pubSubClient
String msgInfo;
char msgInfo_buff[100];  //for topicStoring

//Rfid Initialize
byte readCard[4];
String rfidDetail;
String accessStr;
String jsonRfid;
char jsonRfid_buff[100];

// RFID Pin Selection
#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup(){
  Serial.begin(9600);
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.println(F("Ethernet Begin :"));
  mfrc522.PCD_Init();

  while(!client.connected()) {
      Serial.println(F("Trying to connect client to server..."));
      //mqttClientId,WillTopic,willQos,willRetain,willMessage);
      client.connect("Arduino1","RU_<DE,AD,BE,FF,06,ED>", 1, true, "Unexpected Disconnect");
      if(client.connected()) {
          Serial.println ("Success!!,"+ myMacId + " Connected! ");
          }
        }

        if(client.connected()) {
          Serial.println("Send data to broker ");
        }
  }
void loop(){
  if(client.connected()) {
    if(Serial.available()){
      char a = Serial.read();
      if ( a == 'b' ){
        digitalWrite(10,HIGH);
        digitalWrite(23,LOW);
        Serial.println(F("Waiting PICCs to be scanned"));
        if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
          return;
          }
        if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
          return;
          }
      }
      Serial.println(F("Scanned PICC's UID:"));
      for (int i = 0; i < 4; i++){  //
        readCard[i] = mfrc522.uid.uidByte[i];
        rfidDetail+=readCard[i];
        Serial.print(readCard[i]);
        Serial.print(" ");
        }
    }
    
  }
   client.loop();
}
//void getID() {
//  // Getting ready for Reading PICCs
//  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
//     return;
//  }
//  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
//      return;
//  }
//  jsonRfid= "{\"RFID \" :\"";
//  Serial.println(F("Scanned PICC's UID:"));
//  for (int i = 0; i < 4; i++){  //
//      readCard[i] = mfrc522.uid.uidByte[i];
//      rfidDetail+=readCard[i];
//      Serial.print(readCard[i]);
//      Serial.print(" ");
//    }
//    jsonRfid+=rfidDetail;
//    jsonRfid+="\"}";
//    jsonRfid.toCharArray(jsonRfid_buff,50);
//    client.subscribe("<WC_CODE>/OPERATOR_AUTH");
//    client.publish("RFID",jsonRfid_buff);
//    Serial.println();
//    Serial.println(jsonRfid_buff);
//    rfidDetail="";
//    jsonRfid="";
//    memset(jsonRfid_buff,0,50);
//    mfrc522.PICC_HaltA(); // Stop reading
//  }
void callback(char* topic,byte* payload,unsigned int length)
  {
    
  }

