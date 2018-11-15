#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>

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


byte mac[] = {0x11, 0x11, 0x11, 0x00, 0x00, 0x01};

byte server[] = {192, 168, 3, 14};
byte ip[] = {192, 168, 3, 34};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 3, 1};


EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

String myMacId = "11-11-11-00-00-01";
String clientId = "RU_" + myMacId;
String clientState = "client/state/RU_" + myMacId;
String getRuDetail = "GET_RU_DETAIL";
String setRuDetail = "SET_RU_DETAIL_" + myMacId;
String rfidSubscribeStr;
String ruidStr;
String errorJson;
String getPayload;
String topicCallback;

//char Buffers
char clientIdBuff[50];
char messageBuff[400];

//Boolean
boolean messageReceived = false;
boolean getRuDetailFlag = false;

void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    tft.fillScreen(White);
    Serial.println(F("TFT Initialized"));
    
    Ethernet.begin(mac, ip, gateway, subnet);
    Serial.println(F("Ethernet Initialized"));
    clientId.toCharArray(clientIdBuff, (clientId.length() + 1));
    delay(2000); // 2sec delay
}

void loop()
{
    if (!client.connected()) {
        Serial.println(F("Trying to connect to MQTT Broker..."));
        tftDisplay("Trying to connect to", 2, Black ,0,80 ,true);
        tftDisplay("MQTT Broker...", 2, Black ,0,120 ,false);
        client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println ("Success!! MQTT:" + myMacId + " Connected! ");
            tftDisplay(myMacId, 2, Black ,0,80 ,true);
            tftDisplay( "Is Connected" , 2, Black ,0,110 ,false);
            tftDisplay( "Successfully!!", 2, Black ,0,140 ,false);
            
            publish(clientState, clientId, true); //also send retainflag = true
            Serial.println("Subscribe to " + setRuDetail);
            subscribe(setRuDetail);
        } else {
            delay(10000);
        }
        getRuDetailFlag = false;
    } else {
        Serial.println("Client Connected");
        client.loop();
        myDelay(10);
    }
   
}

void publish(String topic, String msg, boolean retainFlag)
{
    char topicBuf[100];
    char msgBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    msg.toCharArray(msgBuf, msg.length()+1);
    client.publish(topicBuf, msgBuf, retainFlag);
}

void subscribe(String topic) {
    char topicBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    client.subscribe(topicBuf);   
}

void myDelay(unsigned long sec)
{
    unsigned long delayMSec = sec * 1000;
    unsigned long recordMSec = millis();
    messageReceived = false;
    //while loop will run untill intervel time,then it will comeout
    while(((millis() - recordMSec) < delayMSec) && (!messageReceived)) {
        client.loop();
    }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
    int i=0;
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.println("] ");
    
    for( i=0;i<length;i++){
        messageBuff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    messageBuff[i]='\0';
    tftDisplay((String)messageBuff, 2, Black ,0,80 ,true);
    Serial.print("messageBuff:");
    Serial.println(messageBuff);
    Serial.println();
    messageReceived = true;
}

void tftDisplay(String msg,int textSize,int color,int x, int y, boolean clearFlag)
{
    int msgLength = msg.length();
    tft.setCursor(x,y);
    tft.setTextColor(color);
    if( clearFlag) {
        tft.fillScreen(White);
    }
    center(msg,textSize);
}


void center( String text, byte textSize )
{
    // if textSize(1)--> max char 53
    // if textSize(2)--> max char 26
    // if textSize(3)--> max char 17
    // if textSize(4)--> max char 13
    // if textSize(5)--> max char 10
    char space[6] = {0, 53, 26, 17, 13, 10};
    if(textSize == 0) {
        textSize = 1;
    }
    
    tft.setTextSize(textSize);
    int len = text.length();

    int x= ( space[textSize] / 2 ) - ( len / 2 );
    for(int i=0; i<=x ;i++){
        tft.print(" ");
    }
    tft.println(text);
}

void center( const char* text, int len, byte textSize )
{
    char space[6] = {0, 53, 26, 17, 13, 10};
    if(textSize == 0) {
        textSize = 1;
    }
    
    tft.setTextSize(textSize);
    int x= ( space[textSize] / 2 ) - ( len / 2 );
    for(int i=0; i<=x ;i++){
        tft.print(" ");
    }
    for(int i = 0; i < len; i++)
        tft.print(text[i]);
}

