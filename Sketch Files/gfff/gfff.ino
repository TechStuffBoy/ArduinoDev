
#include<SPI.h>
#include <MFRC522.h>
boolean checkId = false;

#define SS_PIN 23
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);
int ruid = 2;

void setup()
{
    Serial.begin(9600);
    SPI.begin(); 
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized :)"));  
    checkId = true;
}

void loop()
{
    if( checkId == true) {
      getID();
    }
}

void getID()
{
   
    String rfidDetail;
    byte readCard[4];
    String msgInfo;
    char rfidDetailBuff[30];
    char msgInfoBuff[40];
    
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_RESP";
    msgInfo.toCharArray(msgInfoBuff,40);
    
    Serial.println(F("msgInfoBuff Initial :"));
    Serial.println(msgInfoBuff);
    Serial.println();
    msgInfo = "";
    memset(msgInfoBuff,0,40);
    
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_REQ";
    msgInfo.toCharArray(msgInfoBuff,40);
    Serial.println(F("msgInfoBuff Last :"));
    Serial.println(msgInfoBuff);
    Serial.println();
    
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
 
    rfidDetail.toCharArray(rfidDetailBuff,30);
    Serial.println(F("rfidDetailBuff  :"));
    Serial.println(rfidDetailBuff);
    Serial.println();
   
    memset(rfidDetailBuff,0,10);
    mfrc522.PICC_HaltA(); // Stop reading
  }
