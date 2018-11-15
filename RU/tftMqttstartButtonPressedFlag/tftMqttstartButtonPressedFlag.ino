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

#define TS_MINX 125-55
#define TS_MINY 905 //85
#define TS_MAXX 965
#define TS_MAXY 85//905
/*
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
*/
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
int touchAreaIndex = 0;
String touchStr;
struct touchAreaSt {
    int x1;
    int x2;
    int y1;
    int y2;
    String str;
};              
struct touchAreaSt touchArea[10];

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
String getWolStr = "GET_WOL";
String getWolMsgStr = "{\n\"GET_WOL\" :\n{\n\"PLANT\" : \"MUP1\",\n\"WORKCENTER\" : \"P1W1\",\n\"START_DATE\" : \"01-01-1970\",\n\"END_DATE\" :\"01-01-1970\"\n}\n}";
String setWolStr = "SET_WOL";
String pONo1Str;
String materialForPONo1Str;
String pONo2Str;
String materialForPONo2Str;
unsigned int quantityForPONo1 = 0;
unsigned int quantityForPONo2 = 0;
boolean isFirstTime = false;

//char Buffers
char clientIdBuff[50];
char messageBuff[500];

//Boolean
boolean messageReceived = false;
boolean getRuDetailFlag = false;
boolean ruIdErrorFlag = false;
boolean authenticationOk = false;
boolean getWorkorderListFlag = false;

int ruidErrorJsonLen = 0;
int currentPage = 0;

enum state {
    CLIENT_NOT_CONNECTED,
    CLIENT_CONNECTED,
    GET_RU_DETAIL,
    GET_AUTHENTICATION,
    GET_PROD_ORDER_LIST,
    GET_PROD_ORDER_DETAIL,
    START_PRODUCTION
}

state curState = CLIENT_NOT_CONNECTED;
state prevState = CLIENT_NOT_CONNECTED;

void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    tft.fillScreen(White);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    Serial.println(F("TFT Initialized"));
    tftDisplay("TFT Initialized", 2, Yellow ,0,80 ,true,Black);
    Ethernet.begin(mac, ip, gateway, subnet);
    Serial.println(F("Ethernet Initialized"));
    tftDisplay("Ethernet Initialized", 2, Yellow ,0,80 ,true,Black);
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized"));
    tftDisplay("RFID Initialized", 2, Black ,0,80 ,true,White);
    clientId.toCharArray(clientIdBuff, (clientId.length() + 1));
    delay(2000); // 2sec delay
    clearTouchButtons();
}


void loop()
{
    if (!client.connected()) {
        Serial.println(F("Trying to connect to MQTT Broker..."));
        tftDisplay("Trying to connect to", 2, Black ,0,80 ,true,White);
        tftDisplay("MQTT Broker...", 2, Black ,0,120 ,false,White);
        client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println ("Success!! MQTT:" + myMacId + " Connected! ");
            tftDisplay(myMacId, 2, Black ,0,80 ,true,White);
            tftDisplay( "is Connected" , 2, Black ,0,120 ,false,White);
            tftDisplay( "Successfully!!", 2, Black ,0,160 ,false,White);
            
            publish(clientState, clientId, true); //also send retainflag = true
            /***
            Serial.println("Subscribe to " + setRuDetail);
            subscribe(setRuDetail);
            ***/
            //For testing only
            getRuDetailFlag = true;
            authenticationOk = true;
            operatorNameStr = "NandaKumar";
            ruidStr = "RU_01";
            operatorWelcomeScreen();
            messageReceived = false;
        } else {
            getRuDetailFlag = false;
            delay(10000);
        }
    } else {
        if( getRuDetailFlag == false ){
            tftDisplay("Trying to Connect",2,Black,0,80, true,White);
            tftDisplay("to the Gateway...",2,Black,0,120, false,White);
            publish(getRuDetail, myMacId, false);
            if(messageReceived == true) {
                if( ruIdErrorFlag == true ){
                    //display ruiderror
                    tftDisplay(errorMsgGlobal,2,Black,0,100, true,White);
                    //10 sec wait
                    myDelay(10);
                } else {
                    ruWelcomeScreen(companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal);
                     getRuDetailFlag = true;
                     rfidSubscribeStr = ruidStr + "/OP_AUTH_RESP";
                     subscribe(rfidSubscribeStr);
                }
                ruIdErrorFlag = false;
                messageReceived = false;
            } else {
                myDelay(10);
            }
        } else {
            //Serial.println("waiting for authentication");
            if(!authenticationOk) {
                publishRFId();
                if(messageReceived == true) {
                    if( authenticationStr == "ACCEPT"){
                        currentPage = 1;
                        operatorWelcomeScreen();
                        authenticationOk = true;
                  } else {
                        currentPage = 11;
                        tftDisplay("RUID: auth Error",2,Black,0,100, true,White);
                        myDelay(10);
                        ruWelcomeScreen(companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal);
                    }
                    //authenticationStr ="";
                    messageReceived = false;
                 }
            } else {
                subscribe(setWolStr);
                if( getWorkorderListFlag == false){
                    if(buttonPressed() == true && touchStr == "OK") {
                        Serial.println(getWolMsgStr);
                        publish(getWolStr,getWolMsgStr,true);
                        Serial.println("Button Pressed.  ret String = " + touchStr);
                        clearTouchButtons();
                        getWorkorderListFlag = true;
                       
                    }   
                } else {
                    if ( topicCallback == setWolStr ) {
                        Serial.print("touchStr :");
                        Serial.println(touchStr);
                            if( buttonPressed() == false && !isFirstTime){
                                 productionOrderScreen1();
                                 isFirstTime = true;
                            }
                            if( buttonPressed() == true && touchStr == "SELECT"){
                                Serial.println("Button Pressed.  ret String = " + touchStr);
                                confirmStartScreen();
                            } else if ( buttonPressed() == true && touchStr == "NEXT" ){
                                //Serial.println("Button Pressed.  ret String = " + touchStr);
                                productionOrderScreen2();
                            } else if (buttonPressed() == true && touchStr == "PREV" ){
                                //Serial.println("Button Pressed.  ret String = " + touchStr);
                                productionOrderScreen1();
                            }
                        
                         }
                
                
                //Serial.println("waiting for  touching Start button");
               //myDelay(10);
            }
            
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
    } else if ( topicCallback == setWolStr ) {
        isFirstTime = false;
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
        const char* pONo1 = root["SET_WOL"][0]["PROD_ORDER_NO"];
        const char* materialForPONo1 = root["SET_WOL"][0]["MATERIAL"];
        quantityForPONo1 = root["SET_WOL"][0]["QUANTITY"];
        Serial.print("quantityForPONo1 :");
        Serial.println(quantityForPONo1);
        pONo1Str = pONo1;
        materialForPONo1Str = materialForPONo1;
        
        
        const char* pONo2 = root["SET_WOL"][1]["PROD_ORDER_NO"];
        const char* materialForPONo2 = root["SET_WOL"][1]["MATERIAL"];
        quantityForPONo2 = root["SET_WOL"][1]["QUANTITY"];
        pONo2Str =pONo2;
        materialForPONo2Str = materialForPONo2;
         
    }
}

void ruWelcomeScreen(String companyDescGlobal,String plantDescGlobal,String workCenter_01_DescGlobal)
{
    tftDisplay("Welcome to",2,Black,0,20, true,White);
    tftDisplay(companyDescGlobal,2,Black,0,60, false,White);
    tftDisplay(plantDescGlobal,2,Black,0,100, false,White);
    tftDisplay(workCenter_01_DescGlobal,2,Black,0,140, false,White);
    tftDisplay("<Swipe Your Employee ID>",2,Black,0,180, false,White);
}

void operatorWelcomeScreen(){
    tftDisplay("WELCOME",2,Black,0,20, true,White);
    tftDisplay(operatorNameStr,2,Black,0,50, false,White);
    tft.setCursor(0, 90);
    center("Press Ok to", 2);
    tft.setCursor(0, 110);
    center("recieve Workorder List", 2);
    createTouchButton("OK",100,155,3,Black,Green,Red);
}
void productionOrderScreen1() 
{
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true,Black);
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false,Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,90, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,85, false , Black);
    tftDisplayNoCenter(pONo1Str , 2,Yellow ,150,90, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,120, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,115, false , Black);
    tftDisplayNoCenter(materialForPONo1Str,2,Yellow ,150,120, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,150, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,145, false , Black);
    tftDisplayNoCenter((String)quantityForPONo1,2,Yellow ,150,150, false , Black);

    clearTouchButtons();
    createTouchButton("SELECT",150,200,2,Black, Green , Red);
    createTouchButton("NEXT",250,200,2,Black, Green , Red);
    
}

void productionOrderScreen2() 
{
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true,Black);
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false,Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,90, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,85, false , Black);
    tftDisplayNoCenter(pONo2Str , 2,Yellow ,150,90, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,120, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,115, false , Black);
    tftDisplayNoCenter(materialForPONo2Str,2,Yellow ,150,120, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,150, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,145, false , Black);
    tftDisplayNoCenter((String)quantityForPONo2,2,Yellow ,150,150, false , Black);

    clearTouchButtons();
    createTouchButton("SELECT",150,200,2,Black, Green , Red);
    createTouchButton("PREV",250,200,2,Black, Green , Red);
    
}

void confirmStartScreen()
{
     tftDisplay("Check Raw Material" , 2,Yellow ,0,30, true , Black);
     tftDisplay("And" , 2,Yellow ,0,60, false , Black);
     tftDisplay("Readiness of the Machine" , 2,Yellow ,0,90, false , Black);
     tftDisplay("Press Start to Begin" , 2,Yellow ,0,120, false , Black);
     tftDisplay("Manufacturing" , 2,Yellow ,0,150, false , Black);
     clearTouchButtons();
     createTouchButton("START",150,200,2,Black, Green , Red);
}

void tftDisplay(String msg,int textSize,int color,int x, int y, boolean clearFlag,int clearScreenColor)
{
    int msgLength = msg.length();
    tft.setCursor(x,y);
    tft.setTextColor(color);
    if( clearFlag) {
        tft.fillScreen(clearScreenColor);
    }
    center(msg,textSize);
}

void tftDisplayNoCenter(String msg,int textSize,int color,int x, int y, boolean clearFlag,int clearScreenColor)
{
    int msgLength = msg.length();
    tft.setCursor(x,y);
    tft.setTextSize(textSize);
    tft.setTextColor(color);
    if( clearFlag) {
        tft.fillScreen(clearScreenColor);
    }
    tft.print(msg);
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

void publishRFId()
{
    byte readCard[4];

    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
        return;
    }
    Serial.println(F("Scanned PICC's UID:"));
    for (int i = 0; i < 4; i++) {
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



void clearTouchButtons()
{
    for(int i=0; i < sizeof(touchArea)/sizeof(touchAreaSt); i++) {
        touchArea[i].x1 = 0;
        touchArea[i].y1 = 0;
        touchArea[i].x2 = 0;
        touchArea[i].y2 = 0;
        touchArea[i].str = "";
    }
    touchAreaIndex = 0;
}

void createTouchButton(String msg,int cursorX,int cursorY,byte textSize,int textColor,int rectColor,int buttonSelectColor)
{
    if(textSize == 0) {
        textSize = 1;
    }
    
    int rectRadius = textSize;
    int len = msg.length();
    int sizeXFactor[] = {0,6,12,18,24,32}; //320/(no of char for textSize)
    int xMargin[] = {0,1,6,6,6,6};
    int sizeYFactor[] = {0,8,16,24,34,40}; //240/(no of lines for textSize)
    int yMargin[] = {0,1,6,6,6,6};
    int sizeX = len * sizeXFactor[textSize];
    int sizeY = sizeYFactor[textSize] ;
    int left = cursorX - xMargin[textSize];
    int top = cursorY - yMargin[textSize];
    int horiSize = (sizeX + 2*xMargin[textSize]);
    int vertSize = (sizeY + 2*yMargin[textSize]);
    touchArea[touchAreaIndex].x1 = left;
    touchArea[touchAreaIndex].y1 = top;
    touchArea[touchAreaIndex].x2 = left + horiSize;
    touchArea[touchAreaIndex].y2 = top + vertSize;
    touchArea[touchAreaIndex].str = msg;
    String str = "Button X1= " + String(left) + " Y1 = " +  String(top) + " x2 = " +  String(touchArea[touchAreaIndex].x2) + " Y2 = " + String(touchArea[touchAreaIndex].y2) + " Msg = " + msg ;
    Serial.println(str);
    touchAreaIndex++;

    tft.fillRoundRect(left, top, horiSize, vertSize, rectRadius, rectColor);
    tft.setCursor((cursorX ), (cursorY ));
    tft.setTextSize(textSize);
    tft.setTextColor( textColor );
    tft.print(msg);
}
boolean buttonPressed() {
    if(touchAreaIndex == 0)
        return false;
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
        p.y = map(p.y, TS_MINY-40, TS_MAXY, 0, tft.height());
        Serial.print("("); Serial.print(p.x);
        Serial.print(", "); Serial.print(p.y);
        Serial.println(")");

        for(int i=0; i < sizeof(touchArea)/sizeof(touchAreaSt); i++) {
             if (( p.x >= touchArea[i].x1 && p.x <= touchArea[i].x2 && p.y >= touchArea[i].y1 && p.y <= touchArea[i].y2 )) {
                touchStr = touchArea[i].str;
                tft.fillRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, 2, Red);
                clearTouchButtons();
                return(true);
             }
        }
    }
    return false;
}


