#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>

// RFID Pin Selection
#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Library For TFT Lcd
#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>
                    /******TFT LCD STUFF *******/
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

//#define YP A3  // must be an analog pin, use "An" notation!
//#define XM A2  // must be an analog pin, use "An" notation!
//#define YM 9   // can be a digital pin
//#define XP 8   // can be a digital pin

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

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
String ruidErrorJson;
String getPayload;
String topicCallback;
String errorMsgGlobal;
String companyDescGlobal;
String workCenter_01_DescGlobal;
String plantDescGlobal;
String workcenterOneCodeStr;
String rfidPublishStr;
String rfidDetail;
String operatorNameStr;
String authenticationStr;

//char Buffers
char clientIdBuff[50];
char messageBuff[500];

//Boolean
boolean messageReceived = false;
boolean getRuDetailFlag = false;
boolean ruIdErrorFlag = false;
boolean authenticationOk = false;

int ruidErrorJsonLen = 0;
int currentPage = 0;
void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    tft.fillScreen(White);
    Serial.println(F("TFT Initialized"));
    tftDisplay("TFT Initialized", 2, Black ,0,80 ,true);
    Ethernet.begin(mac, ip, gateway, subnet);
    Serial.println(F("Ethernet Initialized"));
    tftDisplay("Ethernet Initialized", 2, Black ,0,80 ,true);
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized"));
    tftDisplay("RFID Initialized", 2, Black ,0,80 ,true);
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
            tftDisplay( "is Connected" , 2, Black ,0,120 ,false);
            tftDisplay( "Successfully!!", 2, Black ,0,160 ,false);
            
            publish(clientState, clientId, true); //also send retainflag = true
            Serial.println("Subscribe to " + setRuDetail);
            subscribe(setRuDetail);
            messageReceived = false;
        } else {
            delay(10000);
        }
        getRuDetailFlag = false;
    } else {
        if( getRuDetailFlag == false ){
            tftDisplay("Trying to Connect",2,Black,0,80, true);
            tftDisplay("to the Gateway...",2,Black,0,120, false);
            publish(getRuDetail, myMacId, false);
            if(messageReceived == true) {
                if( ruIdErrorFlag == true ){
                    //display ruiderror
                    tftDisplay(errorMsgGlobal,2,Black,0,100, true);
                    //10 sec wait
                    myDelay(10);
                } else {
                    ruWelcomeScreen(companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal);
                     getRuDetailFlag = true;

                }
                ruIdErrorFlag = false;
                messageReceived = false;
            } else {
                myDelay(10);
            }
        } else {
            //Serial.println("waiting for authentication");
            if(!authenticationOk) {
                getId();
                 if(messageReceived == true) {
                    if( authenticationStr == "ACCEPT"){
                        currentPage = 1;
                        operatorWelcomeScreen();
                        authenticationOk = true;
                    } else {
                        currentPage = 11;
                        tftDisplay("RUID: auth Error",2,Black,0,100, true);
                        myDelay(10);
                        ruWelcomeScreen(companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal);
                    }
                    //authenticationStr ="";
                    messageReceived = false;
                 }
            } else {
                if (currentPage == 1) {
                    TSPoint p = ts.getPoint();
                    pinMode(XM, OUTPUT);
                    pinMode(YP, OUTPUT);
                    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
                        p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
                        p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
//                        Serial.print("("); Serial.print(p.x);
//                        Serial.print(", "); Serial.print(p.y);
//                        Serial.println(")");
                        if ( p.x >= 75 && p.x <= 200 && p.y >= 50 && p.y <= 80 ) {
                            Serial.println("Touching Start Button");
                            tft.drawRoundRect(100, 155, 135, 35, 4, Red);
                        }
                        if (!( p.x >= 75 && p.x <= 200 && p.y >= 50 && p.y <= 80 )) {
                            tft.drawRoundRect(100, 155, 135, 35, 4, Green);
                        }
                    }
        }
                //Serial.println("waiting for  touching Start button");
               //myDelay(10);
            }
            
        }
    }

    
    client.loop();
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
    topicCallback = topic;
    for( i=0;i<length;i++){
        messageBuff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    messageBuff[i]='\0';
    Serial.print("messageBuff:");
    Serial.println(messageBuff);
    Serial.println();
    messageReceived = true;

    StaticJsonBuffer<450> jsonBuffer;

    if (topicCallback == setRuDetail) {
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
        const char* errorMsg = root["ERROR"]["MESSAGE"];
        const char* ruid = root["RUID"]["ID"];
        const char* companyCode = root["COMPANY"]["CODE"];
        const char* companyDesc = root["COMPANY"]["DESC"];
        const char* plantCode = root["PLANT"]["CODE"];
        const char* plantDesc = root["PLANT"]["DESC"];
        const char* workcenterOneCode = root["WORKCENTER"][0]["CODE"];
        const char* workcenterOneDesc = root["WORKCENTER"][0]["DESC"];
        const char* workcenterTwoCode = root["WORKCENTER"][1]["CODE"];
        const char* workcenterTwoDesc = root["WORKCENTER"][1]["DESC"];
        const char* workcenterThreeCode = root["WORKCENTER"][2]["CODE"];
        const char* workcenterThreeDesc = root["WORKCENTER"][2]["DESC"];
        ruidErrorJson = errorMsg;
        ruidErrorJsonLen = ruidErrorJson.length();
        if(!ruidErrorJsonLen ) {
            companyDescGlobal = companyDesc;
            workCenter_01_DescGlobal = workcenterOneDesc;
            plantDescGlobal = plantDesc;
            workcenterOneCodeStr = workcenterOneCode;
            ruidStr = ruid;
            ruIdErrorFlag = false;
        } else {
             errorMsgGlobal = ruidErrorJson;
             ruIdErrorFlag = true;
        }
    } else if (topicCallback == rfidSubscribeStr) {
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
        const char* operatorId = root ["OPERATOR_ID"];
        const char* operatorName = root["OPERATOR_NAME"];
        const char* authentication = root["AUTHENTICATION"];
        operatorNameStr = operatorName;
        authenticationStr = authentication;
        Serial.print("operatorNameStr :");
        Serial.println(operatorNameStr);
        Serial.print("authenticationStr :");
        Serial.println(authenticationStr);
        authenticationOk = false;
    }
}


void ruWelcomeScreen(String companyDescGlobal,String plantDescGlobal,String workCenter_01_DescGlobal)
{
    tftDisplay("Welcome to",2,Black,0,20, true);
    tftDisplay(companyDescGlobal,2,Black,0,60, false);
    tftDisplay(plantDescGlobal,2,Black,0,100, false);
    tftDisplay(workCenter_01_DescGlobal,2,Black,0,140, false);
    tftDisplay("<Swipe Your Employee ID>",2,Black,0,180, false);
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

void AskRuDetail(int ruidErrorJsonLen)
{
   
    publish(getRuDetail, myMacId, true);
    myDelay(10);
    if ( messageReceived && topicCallback == setRuDetail && !ruidErrorJsonLen) {
        getRuDetailFlag = false;
    }
    topicCallback = "";
}
void getId()
{
    byte readCard[4];
    rfidSubscribeStr = ruidStr + "/OP_AUTH_RESP";
    subscribe(rfidSubscribeStr);
   
    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return;
    }
    Serial.println(F("Scanned PICC's UID:"));
    for (int i = 0; i < 4; i++) { //
    readCard[i] = mfrc522.uid.uidByte[i];
    rfidDetail += readCard[i];
    Serial.print(readCard[i]);
    Serial.print(" ");
    }
    rfidPublishStr = ruidStr + "/OP_AUTH_REQ";
    publish(rfidPublishStr, rfidDetail, true);
    rfidDetail = "";
    mfrc522.PICC_HaltA(); // Stop reading
}
void operatorWelcomeScreen(){
    currentPage = 1;
    tftDisplay("WELCOME",2,Black,0,20, true);
    tftDisplay(operatorNameStr,2,Black,0,50, false);
    tft.setCursor(0, 90);
    center("Press Ok to", 2);
    tft.setCursor(0, 110);
    center("recieve Workorder List", 2);
    tft.fillRoundRect(100, 155, 135, 35, 4, Green);
    tft.setCursor(5, 160);
    center("OK", 3);
}

