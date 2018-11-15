//Include Necessary Files
#include <SPI.h>      
#include <MFRC522.h>

//Byte array to store UID
byte readCard[4];

// Defining pins for RC522
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// use this for Sd Card
char filename[] = "XXXXXXXXXXXXXXX.DAT";  // Stores variable filename
char extension[] = "DAT";          // sometimes the extension gets modified
char dir[] = "/PICCS/";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); 
  ShowReaderDetails();
  Serial.println(F("Waiting PICCs to be scanned"));
}

void loop(){
 getID();
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
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return;
  }
  Serial.println(F("Scanned PICC's UID:"));
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
//  Serial.print("\t\t");
//  Serial.println(readCard[0]);
// We have to check the value of tag not HEX value
//Check your card val and put it here
  if(readCard[0]== 228 && readCard[1]== 208 && readCard[2]== 23 && readCard[3]== 184 ){
    // Do your Functions if card is good
    Serial.println("Operator_two : Anu ");
  }
 else if(readCard[0]== 170 && readCard[1]== 207 && readCard[2]== 112 && readCard[3]== 224 ){
  // Do your Functions if card is good
  Serial.println("Operator_one : Ram ");
}

  else{
    // If card is not valid ,it will come here.
    Serial.println("Unknown ID : Access Denied" );  
  }
  mfrc522.PICC_HaltA(); // Stop reading
  getFilename();    // Get data ready
  }


  void getFilename() {  // We will store UIDs as files on SD card
  sprintf(filename, "%s%02x%02x%02x%02x", dir, readCard[0], readCard[1], // Convert readCard data to char and append extension
          readCard[2], readCard[3], extension);
}
