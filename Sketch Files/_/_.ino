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

// Values For millis() in RU_DETAIL_<MAC_ID>
unsigned long previousMillis1 = 0; // RU_DETAIL_<MAC_ID>
const long interval1 = 3000; // RU_DETAIL_<MAC_ID>

// Values For millis() in errorMessage
unsigned long previousMillis2 = 0;
const long interval2 = 4000;

//Booleans
boolean gatewayConnect = false;
boolean everyFiveSeconds = false;
boolean checkId = false;
boolean errorScreenShowTime = true;
boolean flagSet = false;
boolean showLetter = false;
const char* errorMsgGlobal;

// String
String topicCallback="";
String getPayload="";
String errorJson="";

//intializers 
 int getPayloadLength = 0;
 int currentPage = 0;
 static int loopCnt;
 int errorJsonLen=0;

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
          StartScreen();
       }
}  

void loop()
{
  if(client.connected()) {
    if( gatewayConnect == false ){
      dot(120,130,1,Black);
      }
    if(everyFiveSeconds == true) {
       FiveSecond(errorJsonLen);
      } 
    if( checkId == true ) {
        getID();
    }
    if( flagSet == true ) {
        showLetter = true;
        unsigned long currentMillis2 = millis();
        while(currentMillis2 - previousMillis2 >= interval2) {
            previousMillis2 = currentMillis2;
            showLetter = false;
            tft.fillScreen(White);
            errorScreenShowTime = false;
            flagSet = false;
        }
        if( showLetter == true ) {
            tft.fillScreen(White);
            tft.setTextColor(Black);
            tft.setCursor(0,80);
            center(errorMsgGlobal,2);
            }
      }  
  }

