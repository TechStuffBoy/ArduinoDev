//Include Necessary Files
#include <SPI.h>      
#include <MFRC522.h>

boolean checkId = false;
// Defining pins for RC522
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); 
  ShowReaderDetails();
  checkId = true;
  Serial.println(F("Waiting PICCs to be scanned"));
}

void loop(){
  if( checkId == true) {
      getID();
    }
}

void ShowReaderDetails() {
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
void getID() {
  byte readCard[4];
  char rfidDetailBuff[20];
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return;
  }
  Serial.println(F("Scanned PICC's UID:"));
  String rfidDetail;
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    rfidDetail+=readCard[i];
//    Serial.print(readCard[i]);
//    Serial.print(" ");
  }
  rfidDetail.toCharArray(rfidDetailBuff,20);
  Serial.println(rfidDetailBuff);
  mfrc522.PICC_HaltA();
}
