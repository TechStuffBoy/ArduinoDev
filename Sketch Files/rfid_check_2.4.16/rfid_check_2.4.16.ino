
#include<SPI.h>
#include <MFRC522.h>
boolean checkId = false;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
int ruid = 2;

void setup()
{
    Serial.begin(9600);
   
    SPI.begin();
    
    mfrc522.PCD_Init();
    ShowReaderDetails();
    Serial.println(F("RFID Initialized :)"));  
    checkId = true;
}

void loop()
{
    if( checkId == true) {
      getID();
      
    }
}

void ShowReaderDetails() 
{
  // Get the MFRC522 software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
   }
}
void getID()
{
    String rfidDetail;
    byte readCard[4];
    String msgInfo;
    char rfidDetailBuff[30];
    char msgInfoBuff[40];
    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        //Serial.println("I came to PICC_IsNewCardPresent");
        return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
         //Serial.println("I came to PICC_ReadCardSerial");
        return;
    }
   
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_RESP";
    msgInfo.toCharArray(msgInfoBuff,40);
    Serial.println("<---------- msgInfoBuff1 ------------>");
    Serial.println(msgInfoBuff);
    Serial.println("<---------- msgInfoBuff1_End ------------>");
    msgInfo = "";
    memset(msgInfoBuff,0,40);
  
    
   
    msgInfo += ruid;
    msgInfo += "/OP_AUTH_REQ";
    msgInfo.toCharArray(msgInfoBuff,40);
    Serial.println("<---------- msgInfoBuff2 ------------>");
    Serial.println(msgInfoBuff);
    Serial.println("<---------- msgInfoBuff2_End ------------>");
    
    Serial.println(F("Scanned PICC's UID:"));
    for (int i = 0; i < 4; i++){  //
        readCard[i] = mfrc522.uid.uidByte[i];
        rfidDetail+=readCard[i];
//        Serial.print(readCard[i]);
//        Serial.print(" ");
    }
    rfidDetail.toCharArray(rfidDetailBuff,20);
    Serial.println(rfidDetailBuff);
    mfrc522.PICC_HaltA(); // Stop reading
    checkId = false;
  }
