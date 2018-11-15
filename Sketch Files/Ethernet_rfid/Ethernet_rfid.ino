#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <MFRC522.h> 
String myString;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED
};

byte server[]={192,168,1,14};
byte ip[]={192,168,1,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,1,1};

EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);


byte readCard[4];    // Stores scanned ID read from RFID Module
byte masterCard[4]; 
char filename[] = "XXXXXXXXXXXXXXX.DAT";  // Stores variable filename
char extension[] = "DAT";          // sometimes the extension gets modified
char dir[] = "/PICCS/";

#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
Serial.begin(9600);
Ethernet.begin(mac,ip,gateway,subnet);
Serial.println("Ethernet Begin");
//digitalWrite(10, HIGH);

mfrc522.PCD_Init(); 
ShowReaderDetails();

}
void loop()
{
  while(!client.connected()){
    Serial.println("Trying to connect client to server...");
    client.connect("My_arduinoClient");
    if(client.connected()){ 
      Serial.println("Client Connected!!");
    }
  }
  if(client.connected()){
    //Serial.println("Sent Data to Broker");
    client.publish("outTopic","Hello,World!");
    client.subscribe("inTopic");
     getID();
    delay(50);
    }
    client.loop();
}
  
 void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    
  }
  Serial.print("myString:");
  Serial.println(myString);
  Serial.println();
  
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
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  Serial.println(F("Scanned PICC's UID:"));
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
 // getFilename();    // Get data ready
  }

//void getFilename() {  // We will store UIDs as files on SD card
//  sprintf(filename, "%s%02x%02x%02x%02x.%s", dir, readCard[0], readCard[1], // Convert readCard data to char and append extension
//          readCard[2], readCard[3], extension);
//}
  
