#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <MFRC522.h>
#include <ArduinoJson.h> 

byte readCard[4];
String rfidDetail;
String jsonRfid;
char jsonRfid_buff[50];
char message_buff[50];

// Defining pins for RC522
#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED};

byte server[]={192,168,1,14};
byte ip[]={192,168,1,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,1,1};

EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.println("Ethernet Begin");
  mfrc522.PCD_Init();
  Serial.println(F("Waiting PICCs to be scanned"));
}

void loop(){
   while(!client.connected()){
    Serial.println("Trying to connect client to server...");
    client.connect("My_arduinoClient");
    if(client.connected()){ 
      Serial.println("Client Connected!!");
    }
  }
  if(client.connected()){
    //client.publish("outTopic","Hello,World!");
    //client.subscribe("inTopic");
     getID();
    delay(50);
    }
    client.loop();
}

void getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
     return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
  }
  jsonRfid= "{\"RFID \" :\"";
  Serial.println(F("Scanned PICC's UID:"));
  for (int i = 0; i < 4; i++){  //
      readCard[i] = mfrc522.uid.uidByte[i];
      rfidDetail+=readCard[i];
      Serial.print(readCard[i]);
      Serial.print(" ");
    }
    jsonRfid+=rfidDetail;
    jsonRfid+="\"}";
    jsonRfid.toCharArray(jsonRfid_buff,50);
  client.publish("RFID",jsonRfid_buff);
  Serial.println();
  Serial.println(jsonRfid_buff);
  rfidDetail="";
  jsonRfid="";
  memset(jsonRfid_buff,0,50);
  mfrc522.PICC_HaltA(); // Stop reading
}

void callback(char* topic,byte* payload,unsigned int length){
  int i=0;
  for(i=0;i<length;i++){
    message_buff[i]=payload[i];
  }
  // Hav to stop the string so giving \0
  message_buff[i]='\0';
  Serial.println(message_buff);
}
