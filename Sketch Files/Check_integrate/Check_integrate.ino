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
char message_buff[400];
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

//Constant Strings used in TFTLCD
const char* welcome ="Welcome to";
const char* swipe ="<Swipe Your Employee ID>";


// Values For millis() in Dot Function
unsigned long previousMillis = 0; //for dot
const long interval = 1000; //for dot

//Booleans
boolean arduinoOn = true;
boolean gatewayConnect = false;

// Strings
String getPayload="";
int getPayloadLength = 0;
int i=0;
static int loopCnt;
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
          StartScreen();
          
    }
}

void loop(){
  //Serial.println("I came to loop ");
  if(client.connected()) {
    if ( gatewayConnect == false ){
      //dot(120,130,1,Black,getPayloadLength);
      dot(120,130,1,Black);
    }
         getID();
   }
   client.loop();
  }

void callback(char* topic,byte* payload,unsigned int length)
  {
    //int i=0;
    for(i=0;i<length;i++){
      message_buff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    message_buff[i]='\0';
    getPayload = String(message_buff);
    Serial.println(getPayload);
    getPayloadLength = getPayload.length();
     Serial.print("getPayloadLength :");
     Serial.println(getPayloadLength);
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(message_buff);
    const char* OPERATOR_ID = root["user"]["OPERATOR_ID"];
    const char* OPERATOR_NAME = root["user"]["OPERATOR_NAME"];
    const char* AUTHENTICATION = root["user"]["AUTHENTICATION"];
    const char* errorMsg = root["ERROR"]["MESSAGE"]; 
    const char* companyName = root["COMPANY"]["NAME"];
    int companyId = root["COMPANY"]["ID"];
    const char* plantName = root["PLANT"]["NAME"];
    int plantId = root["PLANT"]["ID"];
    const char* workCenterName_1 = root["WORK_CENTER_1"]["NAME"];
    int workCenterCode_1 = root["WORK_CENTER_1"]["CODE"];
    int workCenterId_1 = root["WORK_CENTER_1"]["ID"];
    int WC_1_RDI_0 = root["WORK_CENTER_1"]["RDI_0"];
    int WC_1_RDO_0 = root["WORK_CENTER_1"]["RDO_0"];
    int WC_1_RAI_0 = root["WORK_CENTER_1"]["RAI_0"];
    const char* workCenterName_2 = root["WORK_CENTER_2"]["NAME"];
    int workCenterCode_2 = root["WORK_CENTER_2"]["CODE"];
    int workCenterId_2 = root["WORK_CENTER_2"]["ID"];
    int WC_2_RDI_0 = root["WORK_CENTER_2"]["RDI_0"];
    int WC_2_RDO_0 = root["WORK_CENTER_2"]["RDO_0"];
    int WC_2_RAI_0 = root["WORK_CENTER_2"]["RAI_0"];
    tft.fillScreen(White);
    tft.setTextColor(Black);
    tft.setCursor(0,20);
    center(welcome,2);
    tft.setCursor(0,60);
    center(companyName,2);
    tft.setCursor(0,100);
    center(plantName,2);
    tft.setCursor(0,140);
    center(workCenterName_1,2);
    tft.setCursor(0,180);
    center(swipe,2);
    
    if(errorMsg){
    tft.fillScreen(White);
    tft.setTextColor(Black);
    tft.setCursor(0,80);
    center(errorMsg,2);
    }
    if(getPayloadLength > 0){
      gatewayConnect = true ;
    }
    getPayload="";
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
  Serial.println(" Iam in Dot");
   if(loopCnt<4){
    Serial.print("wwww:");
    Serial.println(loopCnt);
        unsigned long currentMillis = millis();
        while(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
       
        if(loopCnt==0){
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
    else if(loopCnt>=4){
      loopCnt=0;
   
    }
    loopCnt++;
 }
 
}
}
void StartScreen()
{
    tft.fillScreen(White);
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

