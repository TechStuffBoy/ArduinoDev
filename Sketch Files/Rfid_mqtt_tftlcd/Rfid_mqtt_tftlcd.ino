#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <MFRC522.h>
#include <ArduinoJson.h> 
#include<String.h>

// Library For TFT Lcd
#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>

/******TFT LCD STUFF *******/
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
//Colors 
#define Black           0x0000
#define Blue            0x001F
#define Green           0x07E0
#define Red             0xF800
#define Yellow          0xFFE0 
#define White           0xFFFF
#define Orange          0xFD20
#define Pink            0xF81F
#define MINPRESSURE 10
#define MAXPRESSURE 1000
// Instantiating TouchScreen and LCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define BOXSIZE 40
#define PENRADIUS 3
int i=0;

/******TFT LCD STUFF ENDS *******/

byte readCard[4];
String rfidDetail;
 String accessStr;
String jsonRfid;
char jsonRfid_buff[50];
char message_buff[400];

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
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1);
  tft.fillScreen(White);
  mfrc522.PCD_Init();
  Serial.println(F("Waiting PICCs to be scanned"));

   while(!client.connected()) {
        Serial.println(F("Trying to connect client to server..."));
        //client.connect("Arduino1");
        client.connect("Arduino1","RU_<DE,AD,BE,FF,06,ED>", 1, true, "Unexpected Disconnect");
    if(client.connected()) {
        Serial.println ("Success!!  Connected! ");
    }
   }
    if(client.connected()) {
      Serial.println("Send data to broker");
}

}
void loop(){
  
  if(client.connected()){
    //client.publish("outTopic","Hello,World!");
    client.subscribe("<WC_CODE>/OPERATOR_AUTH");
    getID();
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
    client.subscribe("<WC_CODE>/OPERATOR_AUTH");
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

 StaticJsonBuffer<400> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject(message_buff);

 const char* welcome="Welcome";
 const char* error="ERROR:Auth Failed";
 const char* start="START";
 const char* auth="Press Start to Begin";
 const char* OPERATOR_ID = root["user"]["OPERATOR_ID"];
 const char* OPERATOR_NAME = root["user"]["OPERATOR_NAME"];
 const char* AUTHENTICATION = root["user"]["AUTHENTICATION"];
 Serial.print(F("OPERATOR_ID :"));
 Serial.println(OPERATOR_ID);
 Serial.print(F("OPERATOR_NAME :"));
 Serial.println(OPERATOR_NAME);
 Serial.print(F("AUTHENTICATION :"));
 Serial.println(AUTHENTICATION);
 accessStr = AUTHENTICATION;
 if( accessStr == "ACCEPT" ){
  tft.fillScreen(White);
  tft.setTextColor(Black);
  tft.setCursor(0,20);
  center(welcome,2);
  tft.setCursor(0,50);
  center(OPERATOR_NAME,2);

  tft.setCursor(0,80);
  center(auth,2);

  tft.setCursor(0,120);
  center(start,3);
  
//  Serial.println(F("Welcome"));
//  Serial.println(userName);
//  Serial.println(F("You Are Authorized To Operate :)"));
 }
 if(accessStr == "REJECT"){
     tft.fillScreen(White);
     tft.setTextColor(Black);
     tft.setCursor(0,80);
     center(error,2);
     
//  Serial.println(F("Warning !!"));
//  Serial.println(F("UnKnown ID! Access Denied"));
//  Serial.print(F("Your Name and ID is Being Stored For Security Purpose"));
 }
  accessStr="";
}

void center( const char* text, byte textSize )
{
// if textSize(1)--> max char 53
// if textSize(2)--> max char 26
// if textSize(3)--> max char 17
// if textSize(4)--> max char 13
// if textSize(5)--> max char 10
  tft.setTextSize(textSize);
  String words=text;
  char str[50];
  int len;
  
  strcpy(str, text);
  len = strlen(str);
  if( textSize == 1 ){
    int x=0;
    int space= 53;
    x= ( space / 2 ) - ( len / 2 );
    //Serial.println(x);
    for(int i=0; i<=x ;i++){
      tft.print(" ");
    }
    tft.println(str);
   }

    if( textSize == 2 ){
      int x=0;
      int space= 24;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }

    if( textSize == 3 ){
      int x=0;
      int space= 17;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }

    if( textSize == 4 ){
      int x=0;
      int space= 13;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }

    if( textSize == 5 ){
      int x=0;
      int space= 10;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }
    
    memset(str,0,50);
}

