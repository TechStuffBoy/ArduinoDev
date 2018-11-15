#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>

//InputOutputPorts
#define DI_00  37 //proximitySen_1
#define DI_01  36 //proximitySen_2
#define DI_02  35 //cameraSignal
#define DI_03  34
#define DO_00  33 //conveyor
#define DO_01  32 //crankShaft
#define DO_02  31 //stepperMotor
#define DO_03  30

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
struct POList {
    char prodOrderNo[15];
    char material[50];
    int qty;
};
struct POList prodOrderList[10];
struct portStruct {
    byte bitMask;
    String portName;
    byte pin;
    byte portNo;
    boolean curState;
    boolean prevState;
    
};
portStruct diPorts[4] = {
                            {0b00000001, "", DI_00, 0, false, false},
                            {0b00000010, "", DI_01, 1, false, false},
                            {0b00000100, "", DI_02, 2, false, false},
                            {0b00001000, "", DI_03, 3, false, false}
                        };

portStruct doPorts[4] = {
                            {0b00010000, "", DO_00, 0, false, false},
                            {0b00100000, "", DO_01, 1, false, false},
                            {0b01000000, "", DO_02, 2, false, false},
                            {0b10000000, "", DO_03, 3, false, false}
                        };

byte mac[] = {0x11, 0x11, 0x11, 0x00, 0x00, 0x01};

byte server[] = {192, 168, 3, 14};
byte ip[] = {192, 168, 3, 34};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 3, 1};


EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

String ruidStr;
String myMacId = "11-11-11-00-00-01";
String clientId = "RU_" + myMacId;
String clientState = "client/state/RU_" + myMacId;
String getRuDetail = "GET_RU_DETAIL";
String setRuDetail = "SET_RU_DETAIL_" + myMacId;
String rfidSubscribeStr;
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
String getWolStr;
String setRmlStr;
String getRmlStr;
String getWolMsgStr = "{\n\"GET_WOL\" :\n{\n\"PLANT\" : \"MUP1\",\n\"WORKCENTER\" : \"P1W1\",\n\"START_DATE\" : \"01-01-1970\",\n\"END_DATE\" :\"01-01-1970\"\n}\n}";
String setWolStr;
String getProdOrderDetailStr;
String pONo1Str;
String materialForPONo1Str;
String pONo2Str;
String getRmlMsg = "{\n\"GET_RML\" :\n{ \"PROD_ORDER_NO\" : \"000012345678\"\n}}";
String materialForPONo2Str;
unsigned int quantityForPONo1 = 0;
unsigned int quantityForPONo2 = 0;


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
};

state curState = CLIENT_NOT_CONNECTED;
state prevState = CLIENT_NOT_CONNECTED;



int prodOrderListLen;
int index;
char confirmPoPtrArray[15];
char confirmNoPtrArray[15];
 byte readCard[4];
void setup()
{
    Serial.begin(9600);
    SPI.begin();
    DDRC = 0b11110000; //30,31,32,33, 34,35,36,37
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    Serial.println(F("TFT Initialized"));
    tftDisplay("TFT Initialized", 2, Yellow ,0,80 ,true,Black);
    Ethernet.begin(mac, ip, gateway, subnet);
    Serial.println(F("Ethernet Initialized"));
    tftDisplay("Ethernet Initialized", 2, Yellow ,0,80 ,true,Black);
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized"));
    tftDisplay("RFID Initialized", 2, Yellow ,0,80 ,true,Black);
    clientId.toCharArray(clientIdBuff, (clientId.length() + 1));
    delay(2000); // 2sec delay
    clearTouchButtons();
}


void loop()
{
    switch(curState) {
        case CLIENT_NOT_CONNECTED:
            Serial.println(" I am in CLIENT_NOT_CONNECTED ");
            Serial.println(F("Trying to connect to MQTT Broker..."));
            tftDisplay("Trying to connect to", 2, Yellow ,0,80 ,true,Black);
            tftDisplay("MQTT Broker...", 2, Yellow ,0,120 ,false,Black);
            client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
            if(client.connected()) {
                curState = CLIENT_CONNECTED;
            } else {
                delay(10000);
              }
            break;
        case CLIENT_CONNECTED:
            Serial.println(" I am in CLIENT_CONNECTED ");
            Serial.println ("Success!! MQTT:" + myMacId + " Connected! ");
            tftDisplay(myMacId, 2, Yellow ,0,80 ,true,Black);
            tftDisplay( "is Connected" , 2, Yellow ,0,120 ,false,Black);
            tftDisplay( "Successfully!!", 2, Yellow ,0,160 ,false,Black);
            publish(clientState, clientId, true); //also send retainflag = true
            prevState = curState;
            curState = GET_RU_DETAIL;
            break;
        case GET_RU_DETAIL:
            if(prevState == CLIENT_CONNECTED) {
                Serial.println(" I am in prevState GET_RU_DETAIL ");
                Serial.println("Subscribe to " + setRuDetail);
                subscribe(setRuDetail);
                tftDisplay("Waiting For RU Detail", 2, Yellow ,0,80 ,true,Black);
                prevState = curState;
            } else {
                Serial.println(" I am in GET_RU_DETAIL ");
                publish(getRuDetail, myMacId, false);
                if(messageReceived == true) {
                    messageReceived = false;
                    if( ruIdErrorFlag == true ){
                        //display ruiderror
                        tftDisplay(errorMsgGlobal,2,Yellow,0,100, true,Black);
                        //10 sec wait
                        myDelay(10);
                        tftDisplay("Waiting For RU Detail", 2, Yellow ,0,80 ,true,Black);
                    } else {
                         initPorts();
                         unsubscribe(setRuDetail);
                         curState = GET_AUTHENTICATION;
                    }
                } else {
                    myDelay(10);
                }
            }
            break;
        case GET_AUTHENTICATION:
            if(prevState == GET_RU_DETAIL) {
                 Serial.println(" I am in prevState GET_AUTHENTICATION ");
                 ruWelcomeScreen(companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal);
                 rfidSubscribeStr = ruidStr + "/OP_AUTH_RESP";
                 subscribe(rfidSubscribeStr);
                 prevState = curState;
            } else {
                 Serial.println(" I am in  GET_AUTHENTICATION ");
                 publishRFId();
                 if(messageReceived == true) {
                    messageReceived = false;
                    if( authenticationStr == "ACCEPT"){
                        operatorWelcomeScreen();
                        while(buttonPressed() == false);
                        unsubscribe(rfidSubscribeStr);
                        curState = GET_PROD_ORDER_LIST;
                    } else {
                        tftDisplay("RUID: auth Error",2,Yellow,0,100, true,Black);
                        delay(5000);
                        ruWelcomeScreen(companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal);
                    }
                 }
            }
            break;
        case GET_PROD_ORDER_LIST:

            if(prevState == GET_AUTHENTICATION) {
                Serial.println(" I am in prevState GET_PROD_ORDER_LIST ");
                subscribe(setWolStr);
                Serial.println(getWolMsgStr);
                publish(getWolStr,getWolMsgStr,true);
                prevState = curState;
                index = 0;
                
            } else {
                Serial.println(" I am in  GET_PROD_ORDER_LIST ");
                Serial.println(topicCallback);
                if(messageReceived == true) {
                    messageReceived = false;
                    if (topicCallback == setWolStr) {
                        //unsubscribe(setWolStr);
                        while(1) {
                            boolean yesPressed = false;
                            productionOrderScreen(index);
                            while(buttonPressed() == false);
                            if(touchStr == "NEXT")
                                index++;
                            if(touchStr == "PREV")
                                index--;
                            if(touchStr == "SELECT") {
                                Serial.println(" SELECT pressed");
                                getProductionOrderDetailsScreen(index);
                                while(buttonPressed() == false);
                                if(touchStr == "YES") {
                                    Serial.println("YES PRESSED");
                                    curState = GET_PROD_ORDER_DETAIL;
                                    yesPressed = true;
                               }
                               if(yesPressed == true) {
                                  Serial.println("Breaking while loop");
                                  break;
                               }
                                        
                            }
                     }
                } 
             } else {
                    // we hav to subscribe(setWolStr);
                    Serial.println(" waiting for message in  GET_PROD_ORDER_LIST");
                    myDelay(10);
                  }
                 
           }
          break;
        case GET_PROD_ORDER_DETAIL:
              if(prevState == GET_PROD_ORDER_LIST) {
                  Serial.println(" Entering prevstate GET_PROD_ORDER_DETAIL ");
                  unsubscribe(setWolStr); // changed 
                  subscribe(setRmlStr);
                  publish(getRmlStr,getRmlMsg,true);
                  Serial.println(" getRmlMsg :");
                  Serial.println(getRmlMsg);
                  prevState = curState;
              } else {
                    Serial.println(" Entering  GET_PROD_ORDER_DETAIL ");
                    if(messageReceived == true) {
                        messageReceived = false;
                        if (topicCallback == setRmlStr) {
                            confirmStartScreen();
                            while(buttonPressed() == false);
                            curState = START_PRODUCTION;
                        }
                    } else {
                        myDelay(10);
                    }
              }
              break;
        case START_PRODUCTION:
            if( prevState == GET_PROD_ORDER_DETAIL ) {
                unsubscribe(setRmlStr); //change
                Serial.println("I am in prevState START_PRODUCTION");
                clearTouchButtons();
                tftDisplay("Production Started",2,Yellow,0,100, true,Black);
                prevState = curState;
                Serial.println("Going to START_PRODUCTION"); 
            } else {
                
                readDIPorts();
                //myDelay(10);
            }
            break;
        default:
            break;
        
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

void unsubscribe(String topic) {
    char topicBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    client.unsubscribe(topicBuf);   
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
            //Subscribe and publish topics
            getWolStr = ruidStr + "/GET_WOL";
            setRmlStr = ruidStr +  "/SET_RML";
            getRmlStr = ruidStr +  "/GET_RML";
            setWolStr = ruidStr + "/SET_WOL";
           
            Serial.println(getWolStr);
            Serial.println(setRmlStr);
            Serial.println(getRmlStr);
            Serial.println(setWolStr);
            getProdOrderDetailStr = ruidStr + "/GET_POD";
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
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
        JsonArray& array = root["SET_WOL"];
        prodOrderListLen = 0;
        for (int i = 0; i < array.size(); i++) {
            const char* poPtr= array[i]["PROD_ORDER_NO"];
            const char* matPtr = array[i]["MATERIAL"];
            strcpy(prodOrderList[i].prodOrderNo, poPtr);
            strcpy(prodOrderList[i].material, matPtr);
            prodOrderList[i].qty = array[i]["QUANTITY"];
            Serial.print("OrderNo = ");
            Serial.println(prodOrderList[i].prodOrderNo);
            Serial.print("Material = ");
            Serial.println(prodOrderList[i].material);
            Serial.print("qty = ");
            Serial.println(prodOrderList[i].qty);
            prodOrderListLen++;
        }
    } else if ( topicCallback == setRmlStr ) {
        JsonObject& root = jsonBuffer.parseObject(messageBuff);
        const char* confirmPoPtr= root["SET_RML"]["PROD_ORDER_NO"];
        const char* confirmNoPtr = root["SET_RML"]["CONF_NO"];
        strcpy(confirmPoPtrArray, confirmPoPtr);
        strcpy(confirmNoPtrArray, confirmNoPtr);
        Serial.print("confirmPoPtr = ");
        Serial.println(confirmPoPtrArray);
        Serial.print("confirmNoPtr = ");
        Serial.println(confirmNoPtrArray);
    }
     for( int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
        if( topicCallback == doPorts[i].portName ){
            doPorts[i].prevState = doPorts[i].curState;
            if ( (String)messageBuff == "true")  {
                 doPorts[i].curState = true;
            } else {
                 doPorts[i].curState = false;
              }
           
           if ( doPorts[i].curState != doPorts[i].prevState ) {
             digitalWrite( doPorts[i].pin , doPorts[i].curState );
             Serial.println( "value of" + doPorts[i].portName + (String)doPorts[i].curState );
           }
        }
    } 
}

void ruWelcomeScreen(String companyDescGlobal,String plantDescGlobal,String workCenter_01_DescGlobal)
{
    tftDisplay("Welcome to",2,Yellow,0,20, true,Black);
    tftDisplay(companyDescGlobal,2,Yellow,0,60, false,Black);
    tftDisplay(plantDescGlobal,2,Yellow,0,100, false,Black);
    tftDisplay(workCenter_01_DescGlobal,2,Yellow,0,140, false,Black);
    tftDisplay("<Swipe Your Employee ID>",2,White,0,180, false,Black);
}

void operatorWelcomeScreen(){
    tftDisplay("WELCOME",2,Yellow,0,20, true,Black);
    tftDisplay(operatorNameStr,2,Yellow,0,50, false,Black);
    tft.setCursor(0, 90);
    center("Press Ok to", 2);
    tft.setCursor(0, 110);
    center("recieve Workorder List", 2);
    createTouchButton("OK",150,155,3,Black,Green,Red);
}

void productionOrderScreen(int index) 
{
    Serial.print("In ProdOrderScreen index = ");
    Serial.println(String(index));
    Serial.print("OrderNo = ");
    Serial.println(String(prodOrderList[index].prodOrderNo));
    Serial.print("Material = ");
    Serial.println(String(prodOrderList[index].material));
    Serial.print("qty = ");
    Serial.println(prodOrderList[index].qty);
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true,Black);
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false,Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,90, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,85, false , Black);
    tftDisplayNoCenter(String(prodOrderList[index].prodOrderNo) , 2,Yellow ,150,90, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,120, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,115, false , Black);
    tftDisplayNoCenter(String(prodOrderList[index].material),2,Yellow ,150,120, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,150, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,145, false , Black);
    tftDisplayNoCenter((String)prodOrderList[index].qty,2,Yellow ,150,150, false , Black);

    clearTouchButtons();
    
    if(index != 0)
        createTouchButton("PREV",50,200,2,Black, Green , Red);
    createTouchButton("SELECT",150,200,2,Black, Green , Red);
    if(index != (prodOrderListLen-1))
        createTouchButton("NEXT",250,200,2,Black, Green , Red);
}

void getProductionOrderDetailsScreen(int index)
{
    getRmlMsg = "{\n\"GET_RML\" :\n{ \"PROD_ORDER_NO\" : \"";
    getRmlMsg += prodOrderList[index].prodOrderNo;
    getRmlMsg += "\"\n}}";
    tftDisplay("You Have Selected" , 2,White ,10,10, true , Black);
    
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,50, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,45, false , Black);
    tftDisplayNoCenter(String(prodOrderList[index].prodOrderNo) , 2,Yellow ,150,50, false , Black);
    
    tftDisplayNoCenter("Material" , 2,Yellow ,10,80, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,75, false , Black);
    tftDisplayNoCenter(String(prodOrderList[index].material),2,Yellow ,150,80, false , Black);
    
    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,110, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,105, false , Black);
    tftDisplayNoCenter((String)prodOrderList[index].qty,2,Yellow ,150,110, false , Black);
    
    tftDisplay("Press YES to Confirm",2,Yellow ,0,160, false , Black);
    clearTouchButtons();
    createTouchButton("YES",100,200,2,Black, Green , Red);
    createTouchButton("NO",200,200,2,Black, Green , Red);
    
}

void confirmStartScreen()
{
    
     tftDisplay("Check Raw Material and" , 2,Yellow ,0,30, true , Black);
     tftDisplay("Readiness of the SAP" , 2,Yellow ,0,60, false , Black);
     tftDisplayNoCenter("Conf No " , 2,Yellow ,40,90, false , Black);
     tftDisplayNoCenter(":" , 3,Yellow ,130,90, false , Black);
     tftDisplayNoCenter(String(confirmNoPtrArray) , 2,Red ,150,90, false , Black);
     tftDisplay("Press Start to Begin" , 2,Yellow ,0,120, false , Black);
     tftDisplay("Manufacturing" , 2,Yellow ,0,150, false , Black);
     clearTouchButtons();
     createTouchButton("START",150,200,2,Black, Green , Red);
}
void initPorts()
{
    
  for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
     diPorts[i].portName = ruidStr + "/DI_0"+ i;
     pinMode(diPorts[i].pin ,INPUT);
     Serial.println("inputPin = " + String(i) + "/" +  String(diPorts[i].pin));
  }

  for( int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
     doPorts[i].portName = ruidStr + "/DO_0"+ i;
     pinMode(doPorts[i].pin ,OUTPUT);
     Serial.println("outputPin = " + String(i) + "/" +  String(doPorts[i].pin));
     Serial.println(doPorts[i].portName + "   subscribed");
     subscribe(doPorts[i].portName);
  }
}
void readDIPorts()
{
     boolean val;
     byte recd = PINC;
     //Serial.println("recd = " + String(recd));
     for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        val = (((recd & diPorts[i].bitMask) == 0)? false:true);
        diPorts[i].curState = val;
        //Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
        
        if( val != diPorts[i].prevState ){
            publish(diPorts[i].portName, 
                ((val == true)? "true":"false"),false);
            Serial.println(diPorts[i].portName+ String(     val    ) + "published");  
        }
        diPorts[i].prevState = diPorts[i].curState;
     }
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
   

    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        //Serial.println("I am in publishRFID loop PICC_IsNewCardPresent(");
        return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
       // Serial.println("I am in publishRFID lPICC_ReadCardSerial())");
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
    Serial.println( rfidDetail + "is published");
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
                tft.drawRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, 3, White);
                tft.drawRoundRect(touchArea[i].x1+1, touchArea[i].y1+1, (touchArea[i].x2-touchArea[i].x1)+1, touchArea[i].y2+1 - touchArea[i].y1+1, 3, White);
                clearTouchButtons();
                return(true);
             }
        }
    }
    return false;
}


