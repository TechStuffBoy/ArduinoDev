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
String myMacId = "11-11-11-00-00-01";

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
char msgInfoBuff[100];

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
unsigned long currentMillis;
unsigned long previousMillis = 0; //for dot
const long interval = 1000; //for dot

// Values For millis() in RU_DETAIL_<MAC_ID>
unsigned long currentMillis1;
unsigned long previousMillis1 = 0; // RU_DETAIL_<MAC_ID>
const long interval1 = 3000; // RU_DETAIL_<MAC_ID>

// Values For millis() in ErrorScreen
unsigned long currentMillis2;
unsigned long previousMillis2 = 0; 
const long interval2 = 3000; 

// Values For millis() in ErrorScreen
unsigned long currentMillis3;
unsigned long previousMillis3 = 0; 
const long interval3 = 3000;

//const char* errorMsgGlobal;
//const char* companyDescGlobal;
//const char* plantDescGlobal;
//const char* workCenter_01_DescGlobal;

String errorMsgGlobal="";
String companyDescGlobal="";
String plantDescGlobal="";
String workCenter_01_DescGlobal="";
String clientId;

boolean howManyTime = false;
boolean ErrorScreenFlag = false;
boolean fill = false;
boolean rfidAuthError = false;
boolean fillOneTime = false;


const char* welcome1="Welcome";
const char* error="ERROR:Auth Failed";
const char* start="START";
const char* auth="Press Start to Begin";

void setup()
{
    Serial.begin(9600);
    Ethernet.begin(mac,ip,gateway,subnet);
    Serial.println(F("Ethernet Initialized"));
    mfrc522.PCD_Init();
    Serial.println(F("RFID Initialized"));
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    Serial.println(F("TFT Initialized"));
    everyFiveSeconds = true;
    tft.fillScreen(White);
    clientId = "RU_" + myMacId;
    clientState = "client/state/RU_" + myMacId;
    getRuDetail = "GET_RU_DETAIL";
    getRuDetailLen = getRuDetail.length();
    getRuDetail.toCharArray(getRuDetailMsgBuf, getRuDetailLen);
    setRuDetail = "SET_RU_DETAIL_" + myMacId;
    StartScreen();
}

void loop()
{
    if(!client.connected()) {
        Serial.println(F("Trying to connect to MQTT Broker..."));
        //TFT display
        //parameters required for client.connect = (mqttClientId,WillTopic,willQos,willRetain,willMessage);
        client.connect(clientId, clientId, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println (F("Success!! MQTT:") + myMacId + F(" Connected! "));
            msgInfo = clientId;
            msgInfo.toCharArray(msgInfoBuff, 100);
            client.publish("client/state/" + clientId, msgInfoBuff, true); //also send retainflag = true
            Serial.println(F("Subscribe to RU_DETAIL_<11-11-11-00-00-01>"));
            client.subscribe("SET_RU_DETAIL_" + myMacId);
        } else {
            //Delay for 10 Seconds
        }
        getRuDetailFlag = false;
    } else {
          if(getRuDetailFlag == false) {
            //Keep sending GET_RU_DETAIL request every 10 secs.  Display trying to connect to Gateway
            
            // Delay for 10 secs
          } else {
              if(authenticationOk == false) {
                //Display RU Welcome screen
                //send OP_AUTH_REQ
                //Get the response.
                // if the response is rejected .  Display the error message and wait for 10 seconds and go back to welcome screen
                // If response is okay.  set authenticationOk = true 
                
              } else {
                if(startButtonPressed == false) {
                  // display welcome operator name and provide provision to start
                } else {
                  if(getProductionOrderList == false) {
                    //Get Production Order Numbers message to Gateway
                  } else {
                    if(getProductionOrderInfo == false) {
                      //Get Production order Info
                    } else {
                      if(productionOrderSelected == false) {
                        //display screen in lcd
                       //Operator selects the production order
                      } else {
                        if(confirmationStart == false) {
                          //Display check the raw material and press start
                        } else {
                          //Monitor I/Os and send the Input data to Gateway and received outdata and update the ports
                        }
                      }
                    }
                  }
                }
              }
          }
      
          currentMillis2 = millis(); 
          currentMillis3 = millis();
          
          if( gatewayConnect == false ){
                dot(120,130,1,Black);
          }
          if(everyFiveSeconds == true) {
             FiveSecond(errorJsonLen);
          }   
          if( checkId == true) {
              getID();
          }
          if( showFirstScreen == true ){
              Serial.println("showFirstScreen Starts ");
              Serial.print("companyDescGlobal :");
              Serial.println(companyDescGlobal);
              Serial.print("plantDescGlobal :");
              Serial.println(plantDescGlobal);
              Serial.print("workCenter_01_DescGlobal :");
              Serial.println(workCenter_01_DescGlobal);
              Serial.println("showFirstScreen Ends ");
              tft.fillScreen(White);
              firstScreen(welcome,companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal,swipe);
              checkId = true;
              showFirstScreen = false;
          }
          if( howManyTime == true){
              Serial.println(currentMillis2);
              while( currentMillis2 - previousMillis2 >= interval2 ){
                  if( fill == true){
                      tft.fillScreen(White);
                      fill = false;
                  }
                  Serial.println("i am in condition");
                  StartScreen();
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

          if(currentPage == 1) {
            TSPoint p=ts.getPoint();
            pinMode(XM, OUTPUT);
            pinMode(YP, OUTPUT);
            if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
                p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
                p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
                //Serial.print("("); Serial.print(p.x);
                //Serial.print(", "); Serial.print(p.y);
                //Serial.println(")");
                if( p.x >= 195 && p.x <= 235 && p.y >= 69 && p.y <= 170 ){
                    Serial.println("Touching Start Button");
                    tft.drawRoundRect(100,155,135,35,4,Red);
                }
                if(!( p.x >= 195 && p.x <= 235 && p.y >= 69 && p.y <= 170 )){
                    ft.drawRoundRect(100,155,135,35,4,Green);
                }
            }
          }
          if( fillOneTime == true){
              tft.fillScreen(White);
              fillOneTime = false;
          }
          if( rfidAuthError == true){
              while( currentMillis3 - previousMillis3 >= interval3 ){
                  if( fill == true){
                      tft.fillScreen(White);
                      fill = false;
                  }
                  firstScreen(welcome,companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal,swipe);
                  checkId = true;
                  rfidAuthError = false;
              }
              
              tft.setTextColor(Black);
              tft.setCursor(0,80);
              center(error,2);
          }
          
        }
        client.loop();
 }
void firstScreen(const char* welcome,String companyDesc,String plantDesc,String workCenter_01_Desc,const char* swipe) 
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

void getID() 
{
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
void FiveSecond(int errorJsonLen) 
{
    currentMillis1 = millis();
    msgInfo = "{{\"MAC_ID\" : <11-11-11-00-00-01> }}";
    msgInfo.toCharArray(msgInfoBuff ,100);
    while(currentMillis1 - previousMillis1 >= interval1) {
          previousMillis1 = currentMillis1; // {\"MAC_ID\" :<11-11-11-00-00-01>}
          client.publish("GET_RU_DETAIL", msgInfoBuff);
          msgInfo="";
          memset(msgInfoBuff,0,100);
          }
    if(getPayloadLength > 0 && topicCallback == "RU_DETAIL_<11-11-11-00-00-01>") {
          everyFiveSeconds = false;
    }
    if(getPayloadLength > 0 && topicCallback == "RU_DETAIL_<11-11-11-00-00-01>" && errorJsonLen ) {
          everyFiveSeconds = true;
          errorJsonLen = 0;
          errorJson ="";
      }
    topicCallback=""; 
}

void StartScreen()
{
    tft.setTextColor(Black);
    tft.setCursor(0,60);
    const char* text1="Waiting For Connecting";
    center(text1,2);
    tft.setCursor(0,90);
    const char* text2="to the Gateway";
    center(text2,2);
    Serial.println("Start Screen");
}

void ErrorScreen() 
{
    tft.setTextColor(Black);
    tft.setCursor(0,80);
    center(errorMsgGlobal,2);
}
void dot( int x, int y, byte size, int color)
{
   if(loopCnt<4) {
        currentMillis = millis();
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
void center( String text, byte textSize )
{
    // if textSize(1)--> max char 53
    // if textSize(2)--> max char 26
    // if textSize(3)--> max char 17
    // if textSize(4)--> max char 13
    // if textSize(5)--> max char 10
    char space[6] = {0, 53, 26, 17, 13, 10};
    
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
void callback(char* topic,byte* payload,unsigned int length)
{
    for(i=0;i<length;i++){
        message_buff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    message_buff[i]='\0';
    getPayload = String(message_buff);
    topicCallback = topic;
    getPayloadLength = getPayload.length();
    StaticJsonBuffer<400> jsonBuffer;
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
    //    errorMsgGlobal = errorMsg;
    //    companyDescGlobal = companyDesc;
    //    workCenter_01_DescGlobal = workCenter_01_Desc;
    //    plantDescGlobal = plantDesc;   
    errorJsonLen = errorJson.length();
    if(getPayloadLength > 0){
        gatewayConnect = true ;
    }
    if( getPayloadLength > 0 && topicCallback == "RU_DETAIL_<11-11-11-00-00-01>" && ( !errorJsonLen ) ){
        companyDescGlobal = companyDesc;
        workCenter_01_DescGlobal = workCenter_01_Desc;
        plantDescGlobal = plantDesc;  
        everyFiveSeconds = false;
        ErrorScreenFlag = false;
        showFirstScreen = true;
        client.unsubscribe("RU_DETAIL_<11-11-11-00-00-01>");
        gatewayConnect = true;
        //checkErrorTime= false;
        checkId = true;
    }
    if( getPayloadLength > 0 && topicCallback == "RU_DETAIL_<11-11-11-00-00-01>" && errorJsonLen ){
        // errorJson = errorMsg;
        errorMsgGlobal = errorJson;
        //errorJsonLen = errorJson.length();
        everyFiveSeconds = true;
        showFirstScreen = false;
        previousMillis2 = currentMillis2;
        tft.fillScreen(White);
        howManyTime = true;
        errorScreenFlag = true;
        fill = true;
        checkId = false ;
     }
    if( getPayloadLength > 0 && topicCallback == "<WC_CODE>/OPERATOR_AUTH"){
        Screen2(OPERATOR_ID,OPERATOR_NAME,AUTHENTICATION,welcome,companyDescGlobal,plantDescGlobal,workCenter_01_DescGlobal,swipe);
    }
}
void Screen2(const char* OPERATOR_ID,const char* OPERATOR_NAME,const char* AUTHENTICATION,const char* welcome,String companyDescGlobal,String plantDescGlobal,String workCenter_01_DescGlobal,const char* swipe)
{
    currentPage = 1;
    Serial.println("Screen2 Starts ");
    Serial.print("companyDescGlobal :");
    Serial.println(companyDescGlobal);
    Serial.print("plantDescGlobal :");
    Serial.println(plantDescGlobal);
    Serial.print("workCenter_01_DescGlobal :");
    Serial.println(workCenter_01_DescGlobal);
    Serial.println("Screen2 Ends ");
    accessStr = AUTHENTICATION;
    if(accessStr == "REJECT"){
        currentPage = 11;
        previousMillis3 = currentMillis3;
        fill = true;
        fillOneTime = true;
        rfidAuthError = true;
    } else if( accessStr == "ACCEPT" ){
        rfidAuthError = false;
        checkId = false;
        currentPage = 1;
        showFirstScreen = false;
        tft.fillScreen(White);
        tft.setTextColor(Black);
        tft.setCursor(0,20);
        center(welcome1,2);
        tft.setCursor(0,50);
        center(OPERATOR_NAME,2);
        tft.setCursor(0,80);
        center(auth,2);
        tft.fillRoundRect(100,155,135,35,4,Green);
        tft.setCursor(0,160);
        center(start,3);
    }
    accessStr="";
}  
  
