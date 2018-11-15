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
byte mac[] ={0x11, 0x11, 0x11, 0x00, 0x00, 0x01};


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


//String msgInfo;
//char msgInfoBuff[100];

// RFID Pin Selection
#define SS_PIN 23
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

//String initializations
String topicCallback;
String myMacId = "11-11-11-00-00-01";
String clientId = "RU_" + myMacId;
String clientState = "client/state/RU_" + myMacId;
String getRuDetail = "GET_RU_DETAIL";
String setRuDetail = "SET_RU_DETAIL_" + myMacId;
String rfidSubscribeStr;
String ruidStr;
String errorJson;
String getPayload;

String errorMsgGlobal;
String companyDescGlobal;
String plantDescGlobal;
String workCenter_01_DescGlobal;
String workcenterOneCodeStr;
String rfidDetail;
String rfidPublishStr;
String operatorName;



//char Buffers
char clientIdBuff[50];
char clientStateBuff[50];
char getRuDetailBuff[50];
char setRuDetailBuff[50];
char myMacIdBuff[50];
char rfidSubscribeStrBuff[50];
char messageBuff[450];
char rfidDetailBuff[50];
char rfidPublishStrBuff[50];

//Constant Strings used in TFTLCD
const char* welcome ="Welcome to";
const char* swipe ="<Swipe Your Employee ID>";
const char* welcome1="Welcome";
const char* error="ERROR:Auth Failed";
const char* start="START";
const char* auth="Press Start to Begin";

//Boolean
boolean getRuDetailFlag = false;
boolean authenticationOk = false;
boolean startButtonPressed = false;
boolean getProductionOrderList = false;
boolean getProductionOrderInfo = false;
boolean productionOrderSelected = false;
boolean confirmStart = false;

boolean gatewayConnect = false;
boolean reconnectMqttFlag = false;
boolean ruWelcomeScreenFlag = false;
boolean checkId = false;
boolean rfidRejectFlag = false;
boolean operatorErrorFlag = false;
boolean fill = false;
boolean fillOneTime = false;
boolean rfidAuthError = false;
boolean howManyTime = false;
boolean ErrorScreenFlag = false;
boolean operatorWelcomeScreenFlag = false;

int getPayloadLength = 0;
int errorJsonLen = 0;
static int loopCnt;
byte currentPage = 0;

unsigned long currentMillisToReconnectMqtt;
unsigned long previousMillisToReconnectMqtt = 0;
unsigned int intervalToReconnectMqtt = 10000;

unsigned long currentMillisToAskRuDetail;
unsigned long previousMillisToAskRuDetail = 0;
unsigned int intervalToToAskRuDetail = 10000;

unsigned long currentMillisToDot;
unsigned long previousMillisToDot = 0;
unsigned int intervalToDot = 1000;

unsigned long currentMillisToScreen2;
unsigned long previousMillisToScreen2;
unsigned int intervalToScreen2 = 10000;

unsigned long currentMillis2;
unsigned long previousMillis2 = 0;
unsigned int interval2 = 5000;

unsigned long currentMillis3;
unsigned long previousMillis3 = 0;
unsigned int interval3 = 5000;
void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    tft.fillScreen(White);
    Serial.println(F("TFT Initialized"));   
    
    Ethernet.begin(mac,ip,gateway,subnet);
    Serial.println(F("Ethernet Initialized"));
    
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized"));


    //Strings to char array
    myMacId.toCharArray(myMacIdBuff,(myMacId.length()+1));
    clientId.toCharArray(clientIdBuff,(clientId.length()+1));
    clientState.toCharArray(clientStateBuff,(clientState.length()+1));
    getRuDetail.toCharArray(getRuDetailBuff,(getRuDetail.length()+1));
    setRuDetail.toCharArray(setRuDetailBuff,(setRuDetail.length()+1));

}
void loop()
{  
  currentMillisToReconnectMqtt = millis();
  currentMillisToScreen2 = millis();
  currentMillis2 = millis();
  currentMillis3 = millis();
       
   if(!client.connected()) {
        Serial.println(F("Trying to connect to MQTT Broker..."));
        //TFT display
         //parameters required for client.connect = (mqttClientId,WillTopic,willQos,willRetain,willMessage);
        client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println (("Success!! MQTT:" + clientId + "Connected! "));
            client.publish(clientStateBuff, clientIdBuff, true); //also send retainflag = true
            Serial.println(F("Subscribe to SET_RU_DETAIL_11-11-11-00-00-01"));
            client.subscribe(setRuDetailBuff);
        } else {
          //Delay for 10 Seconds
          previousMillisToReconnectMqtt = currentMillisToReconnectMqtt;
          reconnectMqttFlag = true;
        }
        getRuDetailFlag = false;
   } else {
          if(getRuDetailFlag == false) {
            //Keep sending GET_RU_DETAIL request every 10 secs.  Display trying to connect to Gateway
             GatewayConnectScreen();
             gatewayConnect = false;
             AskRuDetail(errorJsonLen);
             // Delay for 10 secs
          } else {
               if(authenticationOk == false) {
                //Display RU Welcome screen
                //send OP_AUTH_REQ
                //Get the response.
                // if the response is rejected .  Display the error message and wait for 10 seconds and go back to welcome screen
                // If response is okay.  set authenticationOk = true 
                ruWelcomeScreenFlag = true;
                checkId = true;
             } else {
               if(startButtonPressed == false) {
                operatorWelcomeScreenFlag = true;
               } else {
             }
            }
     }
     //
     
     if( reconnectMqttFlag == true ){
     while(currentMillisToReconnectMqtt - previousMillisToReconnectMqtt >= intervalToReconnectMqtt) {       
          previousMillisToReconnectMqtt = currentMillisToReconnectMqtt;
           //Serial.println("I am in loop");
           client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
           reconnectMqttFlag = false;
          }
     }
    if( gatewayConnect == false ){
       dot(120,130,1,Black);
      }
    if( ruWelcomeScreenFlag == true ){
        tft.fillScreen(White);
        RuWelcomeScreen(welcome,companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal,swipe);
        checkId = true;
        ruWelcomeScreenFlag = false;
    }
   if( checkId == true ){
        getId();
   }
   if( operatorWelcomeScreenFlag == true) {
    OperatorWelcomeScreen();
   }
   
   if( fillOneTime == true){
              tft.fillScreen(White);
              fillOneTime = false;
          }
          if( rfidAuthError == true) {
              while( currentMillis3 - previousMillis3 >= interval3 ){
                  if( fill == true){
                      tft.fillScreen(White);
                      fill = false;
                  }
                  ruWelcomeScreenFlag = true;
                  //RuWelcomeScreen(welcome,companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal,swipe);
                  checkId = true;
                  rfidAuthError = false;
              }
              
              tft.setTextColor(Black);
              tft.setCursor(0,80);
              center(error,2);
          }
        if( howManyTime == true){
              Serial.println(currentMillis2);
              while( currentMillis2 - previousMillis2 >= interval2 ){
                  if( fill == true){
                      tft.fillScreen(White);
                      fill = false;
                  }
                  Serial.println("i am in condition");
                  GatewayConnectScreen();
                  gatewayConnect = false;
                  howManyTime = false;
                  break;
              } 
              while( ErrorScreenFlag == true ){
                  gatewayConnect = true;
                  ErrorScreen();
                  ErrorScreenFlag = false;
              }
          } 
}
}
void callback(char* topic,byte* payload,unsigned int length)
{
    for(i=0;i<length;i++){
        messageBuff[i]=payload[i];
    }
    messageBuff[i]='\0';
   
    getPayload = messageBuff;
    topicCallback = topic;
    Serial.print(" Topic:");
    Serial.println(topicCallback);
    Serial.print(" payload:");
    Serial.println(messageBuff);
    getPayloadLength = getPayload.length();
    StaticJsonBuffer<450> jsonBuffer;
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
    ruidStr = ruid;
    workcenterOneCodeStr = workcenterOneCode;
    errorJson = errorMsg;
    errorJsonLen = errorJson.length();
    
   
    if(getPayloadLength > 0){
        gatewayConnect = true ;
    }
    Serial.println("In callback");
    Serial.print("setRuDetailBuff :");
    Serial.println(setRuDetailBuff);
    if( (getPayloadLength > 0) && (topicCallback == setRuDetailBuff) && ( !errorJsonLen ) ){
        getRuDetailFlag = true;
        companyDescGlobal = companyDesc;
        workCenter_01_DescGlobal = workcenterOneDesc;
        plantDescGlobal = plantDesc;  
     }
    if( (getPayloadLength > 0) && (topicCallback == setRuDetailBuff) && (errorJsonLen) ){
        getRuDetailFlag = false;
        errorMsgGlobal = errorJson;
        ruWelcomeScreenFlag = false;
        previousMillis2 = currentMillis2;
        tft.fillScreen(White);
        howManyTime = true;
        ErrorScreenFlag = true;
        fill = true;
        checkId = false ;
     }
      if( getPayload == "REJECT" ){
      rfidRejectFlag = true;
       } else{
           rfidRejectFlag = false;
         }
    if( (getPayloadLength > 0) && (rfidRejectFlag) && (topicCallback == rfidPublishStrBuff) ){
        operatorErrorFlag = true;
        Screen2();
     }
    if( (getPayloadLength > 0) && (!rfidRejectFlag) && (topicCallback == rfidPublishStrBuff) ){
        operatorName = getPayload;
        operatorErrorFlag = false; 
        Screen2();
     }  
}
void AskRuDetail(int errorJsonLen) 
{
    currentMillisToAskRuDetail = millis();
    if(currentMillisToAskRuDetail - previousMillisToAskRuDetail >= intervalToToAskRuDetail) {
          previousMillisToAskRuDetail = currentMillisToAskRuDetail; // {\"MAC_ID\" :<11-11-11-00-00-01>}
          client.publish(getRuDetailBuff, myMacIdBuff,true);
    }
    if(getPayloadLength > 0 && topicCallback == setRuDetail) {
          getRuDetailFlag = false;
    }
    if(getPayloadLength > 0 && topicCallback == setRuDetail && errorJsonLen ) {
          getRuDetailFlag = true;
          errorJsonLen = 0;
          errorJson ="";
      }
    topicCallback=""; 
}

void GatewayConnectScreen()
{
    tft.setTextColor(Black);
    tft.setCursor(0,60);
    const char* text1="Waiting For Connecting";
    center(text1,2);
    tft.setCursor(0,90);
    const char* text2="to the Gateway";
    center(text2,2);
    //Serial.println("Start Screen");
}

void dot( int x, int y, byte size, int color)
{
   if(loopCnt<4) {
        currentMillisToDot = millis();
        while(currentMillisToDot - previousMillisToDot >= intervalToDot) {
        previousMillisToDot = currentMillisToDot;
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
         } else if(loopCnt==1){
              tft.setCursor (x,y);
              tft.setTextColor(Black);
              tft.print("*");
              tft.setCursor (x+20,y);
              tft.setTextColor(Black);
              tft.print("*");
              tft.setCursor (x+40,y);
              tft.setTextColor(White);
              tft.print("*");
         } else if(loopCnt==2){
              tft.setCursor (x,y);
              tft.setTextColor(Black);
              tft.print("*");
              tft.setCursor (x+20,y);
              tft.setTextColor(Black);
              tft.print("*");
              tft.setCursor (x+40,y);
              tft.setTextColor(Black);
              tft.print("*");
           } else if(loopCnt == 3){
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

void center( String text, byte textSize )
{
    // if textSize(1)--> max char 53
    // if textSize(2)--> max char 26
    // if textSize(3)--> max char 17
    // if textSize(4)--> max char 13
    // if textSize(5)--> max char 10
    tft.setTextSize(textSize);
    //  String words=text;
    //  char str[50];
    int len;
    
    //strcpy(str, text);
    len = text.length();
    if( textSize == 1 ){
        int x=0;
        int space= 53;
        x= ( space / 2 ) - ( len / 2 );
        //Serial.println(x);
        for(int i=0; i<=x ;i++){
            tft.print(" ");
        }
        tft.println(text);
    }
    
    if( textSize == 2 ){
        int x=0;
        int space= 24;
        x= ( space / 2 ) - ( len / 2 );
        //Serial.println(x);
        for(int i=0; i<=x ;i++){
            tft.print(" ");
        }
        tft.println(text);
    }
    
    if( textSize == 3 ){
        int x=0;
        int space= 17;
        x= ( space / 2 ) - ( len / 2 );
        //Serial.println(x);
        for(int i=0; i<=x ;i++){
            tft.print(" ");
        }
        tft.println(text);
    }
    
    if( textSize == 4 ){
        int x=0;
        int space= 13;
        x= ( space / 2 ) - ( len / 2 );
        //Serial.println(x);
        for(int i=0; i<=x ;i++){
            tft.print(" ");
        }
        tft.println(text);
    }
    
    if( textSize == 5 ){
        int x=0;
        int space= 10;
        x= ( space / 2 ) - ( len / 2 );
        //Serial.println(x);
        for(int i=0; i<=x ;i++){
            tft.print(" ");
        }
        tft.println(text);
    }
    text="";
    // memset(str,0,50);
}

void RuWelcomeScreen(const char* welcome,String companyDesc,String plantDesc,String workCenter_01_Desc,const char* swipe) 
{
    //tft.fillScreen(White);
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
}

void getId() 
{    
     byte readCard[4];
     rfidSubscribeStr = workcenterOneCodeStr + "/OP_AUTH_RESP";
     rfidSubscribeStr.toCharArray(rfidSubscribeStrBuff,(rfidSubscribeStr.length()+1));
     client.subscribe(rfidSubscribeStrBuff);
     rfidPublishStr = ruidStr + "/OP_AUTH_REQ";
     
     rfidPublishStr.toCharArray(rfidPublishStrBuff,(rfidPublishStr.length()+1) );
    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
        return;
    }
    Serial.println(F("Scanned PICC's UID:"));
    for (int i = 0; i < 4; i++){  //
        readCard[i] = mfrc522.uid.uidByte[i];
        rfidDetail+=readCard[i];
        Serial.print(readCard[i]);
        Serial.print(" ");
    }
    rfidDetail.toCharArray(rfidDetailBuff,(rfidDetail.length()+1));
    client.publish(rfidPublishStrBuff,rfidDetailBuff,true);
    rfidDetail="";
    memset(rfidDetailBuff,0,50);
    mfrc522.PICC_HaltA(); // Stop reading
  }
  void Screen2()
{
    currentPage = 1;
    if(operatorErrorFlag){
        currentPage = 11;
        previousMillisToScreen2 = currentMillisToScreen2;
        fill = true;
        fillOneTime = true;
        rfidAuthError = true;
    } else {
        rfidAuthError = false;
        authenticationOk = true;
        checkId = false;
        currentPage = 1;
        ruWelcomeScreenFlag = false;
        operatorWelcomeScreenFlag = true;
    }
    operatorName = "";
}  
void ErrorScreen() 
{
    tft.setTextColor(Black);
    tft.setCursor(0,80);
    center(errorMsgGlobal,2);
}

void OperatorWelcomeScreen()
{
       tft.fillScreen(White);
        tft.setTextColor(Black);
        tft.setCursor(0,20);
        center(welcome1,2);
        tft.setCursor(0,50);
        center(operatorName,2);
        tft.setCursor(0,80);
        center(auth,2);
        tft.fillRoundRect(100,155,135,35,4,Green);
        tft.setCursor(0,160);
        center(start,3);
}       

