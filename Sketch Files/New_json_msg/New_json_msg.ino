#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
char messageBuff[400];
int ruid;

boolean ruidShowFlag = false;
boolean checkId = false;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED};

// LocalServer with MQTT Broker[ Mosquitto]
byte localServer[]={192,168,3,14};

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
    ruidShowFlag = true;
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
   client.publish("GET_RU_DETAIL","11-11-11-00-00-01");
   client.subscribe("SET_RU_DETAIL_11-11-11-00-00-01");
}
void loop()
{  
    if(ruidShowFlag == true){
        Serial.print(F("Ruid in loop :"));
        Serial.println(ruid);
        ruidShowFlag = false;
    }
    if(checkId == true){
        getID();
     }
     
    client.loop();
}

void callback(char* topic,byte* payload,unsigned int length)
{ 
    String topicCallback;
    topicCallback = topic;
    String msgInfo;
    char msgInfoBuff[40];
    String payloadString;
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_RESP";
    msgInfo.toCharArray(msgInfoBuff,40);
    Serial.print(msgInfoBuff);
   
    int i;
    for(i=0;i<length;i++) {
        messageBuff[i]=payload[i];
    }
    messageBuff[i]='\0';
    payloadString = String(messageBuff);
    
    //Serial.println(messageBuff);
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(messageBuff);
    
    const char* companyDesc = root["COMPANY"]["DESC"];
    const char* plantCode = root["PLANT"]["CODE"];
    const char* plantDesc = root["PLANT"]["DESC"];
    const char* workcenterOneCode = root["WORKCENTER"][0]["CODE"];
    const char* workcenterOneDesc = root["WORKCENTER"][0]["DESC"];
    const char* workcenterTwoCode = root["WORKCENTER"][1]["CODE"];
    const char* workcenterTwoDesc = root["WORKCENTER"][1]["DESC"];
    const char* workcenterThreeCode = root["WORKCENTER"][2]["CODE"];
    const char* workcenterThreeDesc = root["WORKCENTER"][2]["DESC"];
    Serial.print(F("topicCallback :"));
    Serial.println(topicCallback);
     if (  topicCallback == "SET_RU_DETAIL_11-11-11-00-00-01" ){
         ruid = root["RUID"]["ID"];
         const char* companyCode = root["COMPANY"]["CODE"];
          ruidShowFlag = true;
          checkId = true;
          
          Serial.print(F("ruid :"));
          Serial.println(ruid);
          
          Serial.print(F("companyCode :"));
          Serial.println(companyCode);
          
          Serial.print(F("companyDesc :"));
          Serial.println(companyDesc);
          
          Serial.print(F("plantCode :"));
          Serial.println(plantCode);
          
          Serial.print(F("plantDesc :"));
          Serial.println(plantDesc);
          
          Serial.print(F("workcenterOneCode :"));
          Serial.println(workcenterOneCode);
          
          Serial.print(F("workcenterOneDesc :"));
          Serial.println(workcenterOneDesc);
    
          Serial.print(F("workcenterTwoCode :"));
          Serial.println(workcenterTwoCode);
          
          Serial.print(F("workcenterTwoDesc :"));
          Serial.println(workcenterTwoDesc);
    
          Serial.print(F("workcenterThreeCode :"));
          Serial.println(workcenterThreeCode);
          
          Serial.print(F("workcenterThreeDesc :"));
          Serial.println(workcenterThreeDesc);
          topicCallback ="";
    }
   
    if( topicCallback == msgInfoBuff ){
      
        Serial.println(F("In callback rfid checkBlock :"));
        Serial.println(F("messageBuff :"));
        Serial.println(payloadString);
        
        while(!(payloadString.equals("REJECT"))){
            Serial.print(F("Welcome "));
            Serial.println(messageBuff);
            //checkId = false;
            Serial.println(F("RFID is Disabled Successfully"));
            payloadString = "";
            break;
        }
        
        while(payloadString.equals("REJECT")){
            Serial.println(F("Warning!, RFID Auth Failed "));
            checkId = true;
            payloadString = "";
            break;
        }
     }
}

void getID() {
    String rfidDetail;
    byte readCard[4];
    String msgInfo;
    char rfidDetailBuff[10];
    char msgInfoBuff[40];
    
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_RESP";
    msgInfo.toCharArray(msgInfoBuff,40);
    
    client.subscribe(msgInfoBuff);
    msgInfo = "";
    memset(msgInfoBuff,0,40);
    
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_REQ";
    msgInfo.toCharArray(msgInfoBuff,40);
    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
        return;
    }
    Serial.println(F("Scanned PICC's UID:"));
    for (int i = 0; i < 4; i++){  //
        readCard[i] = mfrc522.uid.uidByte[i];
        rfidDetail+=readCard[i];
        Serial.print(readCard[i]);
        Serial.print(" ");
    }
   
    rfidDetail.toCharArray(rfidDetailBuff,15);
   
    client.publish(msgInfoBuff,rfidDetailBuff);
    memset(rfidDetailBuff,0,10);
    mfrc522.PICC_HaltA(); // Stop reading
  }
