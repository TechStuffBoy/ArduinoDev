// Library For Ethernet Shield
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>
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

//Buffers For Storing Values
char message_buff[480];
String msgInfo;
char msgInfo_buff[100];

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

//boolean
boolean everyFiveSeconds = false;
boolean gatewayConnect = false;
boolean checkId = false;
boolean showFirstScreen = false;
boolean errorScreenFlag = false;
boolean flag1 = false;
boolean flag1_text = false;

//Constant Strings used in TFTLCD
const char* welcome ="Welcome to";
const char* swipe ="<Swipe Your Employee ID>";

// Strings
 String getPayload="";
 int getPayloadLength = 0;
 int currentPage = 0;
 static int loopCnt;
 String errorJson="";
 int errorJsonLen=0;
 String topicCallback ="";

// Values For millis() in Dot Function
unsigned long previousMillis = 0; //for dot
const long interval = 1000; //for dot

// Values For millis() in RU_DETAIL_<MAC_ID>
unsigned long previousMillis1 = 0; // RU_DETAIL_<MAC_ID>
const long interval1 = 3000; // RU_DETAIL_<MAC_ID>

// Values For millis() in errorScreen
unsigned long previousMillis2 = 0; 
const long interval2 = 3000; 

boolean fill = true;
boolean checkErrorTime = false;

const char* errorMsgGlobal;
const char* companyDescGlobal;
const char* plantDescGlobal;
const char* workCenter_01_DescGlobal;
 
void setup()
{
   Serial.begin(9600);
   Ethernet.begin(mac,ip,gateway,subnet);
   Serial.println(F("Ethernet Begin :"));
   mfrc522.PCD_Init();
   Serial.println(F("Waiting PICCs to be scanned"));
   tft.reset();
   tft.begin(0x9341); // SDFP5408
   tft.setRotation(1);
   while(!client.connected()) {
        Serial.println(F("Trying to connect client to server..."));
        //mqttClientId,WillTopic,willQos,willRetain,willMessage);
        client.connect("Arduino1","RU_<DE,AD,BE,FF,06,ED>", 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println ("Success!!,"+ myMacId + " Connected! ");
        }
   }
   if(client.connected()) {
          Serial.println("Sent Data to Broker");
          msgInfo="{{\"Message\": 1}}";
          msgInfo.toCharArray(msgInfo_buff ,100);
          client.publish("client/state/RU_<DE,AD,BE,FF,06,ED>",msgInfo_buff);
          msgInfo="";
          memset(msgInfo_buff,0,100);
          Serial.println("Subscribed to RU_DETAIL_<DE,AD,BE,FF,06,ED>");
          client.subscribe("RU_DETAIL_<DE,AD,BE,FF,06,ED>");
          everyFiveSeconds = true;
          tft.fillScreen(White);
          StartScreen();
       }
 }

void loop(){
    if(client.connected()) {
    unsigned long currentMills2 = millis(); 
    
        if( gatewayConnect == false ){
          dot(120,130,1,Black);
          }
        if(everyFiveSeconds == true) {
           FiveSecond(errorJsonLen);
          }   
        if( checkId == true ) {
            getID();
        }
        if( showFirstScreen == true ){
          firstScreen(welcome,companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal,swipe);
          checkId = true;
          showFirstScreen = false;
          errorScreenFlag = false;
          checkErrorTime= false;
          flag1 = false;
        }
      
       if( errorScreenFlag == true ){
          tft.fillScreen(White);
          flag1 = true;
         }
        if( flag1 == true ){
          checkErrorTime = true;
          errorScreenFlag = false;
          errorScreen();
          }
    if( checkErrorTime == true ){
    while( currentMills2 - previousMillis2 >= interval2 ){
        //currentMills2 = previousMillis2;
        currentMills2 = 0;
        if( fill == true){
          tft.fillScreen(White);
          fill = false;
        }
        Serial.println("i am in condition");
        flag1 = false;
        StartScreen();
        gatewayConnect = false;
        checkId = false;
        everyFiveSeconds = true;
        checkErrorTime = false;
        
   }
    }
       }
 client.loop();
}
void FiveSecond(int errorJsonLen) 
{
    unsigned long currentMillis1 = millis();
    msgInfo= "{{\"MAC_ID\" : <DE,AD,BE,FF,06,ED> }}";
    msgInfo.toCharArray(msgInfo_buff ,100);
    while(currentMillis1 - previousMillis1 >= interval1) {
          previousMillis1 = currentMillis1; // {\"MAC_ID\" :<DE,AD,BE,FF,06,ED>}
          client.publish("GET_RU_DETAIL",msgInfo_buff);
          msgInfo="";
          memset(msgInfo_buff,0,100);
          }
        
    if(getPayloadLength > 0 && topicCallback == "RU_DETAIL_<DE,AD,BE,FF,06,ED>") {
          everyFiveSeconds = false;
    }
     if(getPayloadLength > 0 && topicCallback == "RU_DETAIL_<DE,AD,BE,FF,06,ED>" && errorJsonLen ) {
          everyFiveSeconds = true;
          errorJsonLen = 0;
          errorJson ="";
      }
    topicCallback=""; 
}

void callback(char* topic,byte* payload,unsigned int length)
  {
    for(i=0;i<length;i++){
      message_buff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    message_buff[i]='\0';
    getPayload = String(message_buff);
    topicCallback = topic;
    Serial.print(" Topic:");
    Serial.println(topicCallback);
    Serial.print(" payload:");
    Serial.println(message_buff);
    getPayloadLength = getPayload.length();
    StaticJsonBuffer<480> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(message_buff);
    const char* OPERATOR_ID = root["user"]["OPERATOR_ID"];
    const char* OPERATOR_NAME = root["user"]["OPERATOR_NAME"];
    const char* AUTHENTICATION = root["user"]["AUTHENTICATION"];
    
    const char* errorMsg = root["ERROR"]["MESSAGE"]; 
    
    const char* ruidId = root["RUID"]["ID"]; 
    const char* macId = root["RUID"]["MACID"]; 
    const char* companyCode = root["COMPANY"]["CODE"]; 
    const char* companyDesc = root["COMPANY"]["DESC"]; 
    const char* plantCode = root["PLANT"]["CODE"];
    const char* plantDesc = root["PLANT"]["DESC"];
    const char* workCenter_01_Code = root["WORKCENTER"]["1"]["CODE"];
    const char* workCenter_01_Desc = root["WORKCENTER"]["1"]["DESC"];
    const char* workCenter_02_Code = root["WORKCENTER"]["2"]["CODE"];
    const char* workCenter_02_Desc = root["WORKCENTER"]["2"]["DESC"];
    const char* workCenter_03_Code = root["WORKCENTER"]["3"]["CODE"];
    const char* workCenter_03_Desc = root["WORKCENTER"]["3"]["DESC"];
    errorJson = errorMsg;
    errorMsgGlobal = errorMsg;
    companyDescGlobal = companyDesc;
    workCenter_01_DescGlobal = workCenter_01_Desc;
    plantDescGlobal = plantDesc;   
    errorJsonLen = errorJson.length();
     if(getPayloadLength > 0){
      gatewayConnect = true ;
    }
   if( getPayloadLength > 0 && topicCallback == "RU_DETAIL_<DE,AD,BE,FF,06,ED>" && ( !errorJsonLen ) ){
       everyFiveSeconds = false;
       errorScreenFlag = false;
       showFirstScreen = true;
       gatewayConnect = true;
       checkErrorTime= false;
       checkId = true;
      }

   if( getPayloadLength > 0 && topicCallback == "RU_DETAIL_<DE,AD,BE,FF,06,ED>" && errorJsonLen ){
      everyFiveSeconds = true;
      showFirstScreen = false;
      //tft.fillScreen(White);
      errorScreenFlag = true;
      fill = true;
      checkId = false ;
      
     }
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
void dot( int x, int y, byte size, int color)
{
  //Serial.println(" Iam in Dot");
   if(loopCnt<4){
//    Serial.print("wwww:");
//    Serial.println(loopCnt);
        unsigned long currentMillis = millis();
        while(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if(loopCnt == 0){
                 tft.setCursor (x,y);
                 tft.setTextColor(Black);
                 tft.print("*");
                 tft.setCursor (x+20,y);
                 tft.setTextColor(White);
                 tft.print("*");
                 tft.setCursor (x+40,y);
                 tft.setTextColor(White);
                 tft.print("*");
                 }
       else if(loopCnt==1){
           tft.setCursor (x,y);
           tft.setTextColor(Black);
           tft.print("*");
           tft.setCursor (x+20,y);
           tft.setTextColor(Black);
           tft.print("*");
           tft.setCursor (x+40,y);
           tft.setTextColor(White);
           tft.print("*");
           }
      else if(loopCnt==2){
           tft.setCursor (x,y);
           tft.setTextColor(Black);
           tft.print("*");
           tft.setCursor (x+20,y);
           tft.setTextColor(Black);
           tft.print("*");
           tft.setCursor (x+40,y);
           tft.setTextColor(Black);
           tft.print("*");
           } 
      else if(loopCnt == 3){
           tft.setCursor (x,y);
           tft.setTextColor(White,White);
           tft.print("*");
           tft.setCursor (x+20,y);
           tft.setTextColor(White,White);
           tft.print("*");
           tft.setCursor (x+40,y);
           tft.setTextColor(White,White);
           tft.print("*");
           }
     loopCnt++;
     if(loopCnt>=4){
       loopCnt=0;
     }
    
 }
}
}

void StartScreen()
{
    //tft.fillScreen(White);
    tft.setTextColor(Black);
    tft.setCursor(0,60);
    const char* text1="Waiting For Connecting";
    center(text1,2);
    tft.setCursor(0,90);
    const char* text2="to the Gateway";
    center(text2,2);
    Serial.println("Start Screen");
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
    jsonRfid.toCharArray(jsonRfid_buff,100);
    client.subscribe("<WC_CODE>/OPERATOR_AUTH");
    client.publish("RFID",jsonRfid_buff);
    Serial.println();
    Serial.println(jsonRfid_buff);
    rfidDetail="";
    jsonRfid="";
    memset(jsonRfid_buff,0,100);
    mfrc522.PICC_HaltA(); // Stop reading
  }

void firstScreen(const char* welcome,const char* companyDesc,const char* plantDesc,const char* workCenter_01_Desc,const char* swipe) {
      tft.fillScreen(White);
      tft.setTextColor(Black);
      tft.setCursor(0,20);
      center(welcome,2);
      tft.setCursor(0,60);
      center(companyDesc,2);
      tft.setCursor(0,100);
      center(plantDesc,2);
      tft.setCursor(0,140);
      center(workCenter_01_Desc,2);
      tft.setCursor(0,180);
      center(swipe,2);
      companyDesc = NULL;
      plantDesc = NULL;
      workCenter_01_Desc = NULL;
}

void errorScreen(){
    //tft.fillScreen(White);
    tft.setTextColor(Black);
    tft.setCursor(0,80);
    center(errorMsgGlobal,2);
    //errorMsgGlobal= NULL;
}
