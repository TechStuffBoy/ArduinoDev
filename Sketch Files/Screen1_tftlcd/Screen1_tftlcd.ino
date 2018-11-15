#include <ArduinoJson.h>
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include<EEPROM.h>
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
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
byte CurrentPage=0;

#define BOXSIZE 40
#define PENRADIUS 3
int i=0;

/******TFT LCD STUFF ENDS *******/
char message_buff[100];
String TopicName;
String id_Topic;
char buff[100];
//byte MyId; // To store Arduino's ID.
byte MyAddr=0; // EEPROM's address (0 -1023), 0 bcs i give id in 0th address
byte MyId=EEPROM.read(MyAddr);
/************ Ehernet and MQTT stuff ***********/

//Arduino Mac Address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED
};
// LocalServer with MQTT Broker[ Mosquitto]
byte localServer[]={192,168,1,14};

//My arduino's Ip Details
byte ip[]={192,168,1,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,1,1};

//Initializing the Client 
EthernetClient ethClient;

//Creating the PubSubClient Object
PubSubClient client(localServer,1883,callback,ethClient);



void setup(){
  Serial.begin(9600);
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.print(F("Arduino ID Is: "));
  Serial.println(MyId);
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1);
  // Screen1();
  initialize();
 
while(!client.connected()){
  Serial.println("Trying to connect client to server...");
  client.connect("Arduino_1");
if(client.connected()){
Serial.println ("Success!!, Client Connected! ");
}
}
if(client.connected()){
   if(readVcc()){
    //Json string
   // Serial.print(MyId);
    //Serial.println(F(" ON"));
     TopicName="RDR_00/";
     id_Topic=TopicName+ MyId;
     id_Topic.toCharArray(buff,100); // RDR_00/MyId
     client.publish( buff , "true");
    // Serial.print("----------------");
    //Serial.println(buff);
    }
    memset(buff,0,100); 
    TopicName="";
    id_Topic="";
//    TopicName="WC_DETAILS/";
//    id_Topic=TopicName+MyId;
//    id_Topic.toCharArray(buff,100);
//    client.subscribe(buff);//WC_DETAILS/MyId
   }
}
void callback(char* topic,byte* payload,unsigned int length){
  int i=0;
  for(i=0;i<length;i++){
    message_buff[i]=payload[i];
  }
  // Hav to stop the string so giving \0
  message_buff[i]='\0';

 String msgString=String(message_buff);
  //Serial.println("PayLoad: " +msgString);
 Serial.println(message_buff);

 StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject(message_buff);
 const char* COMPANY = root["COMPANY"];
 const char* LTD = root["LTD"];
 const char* PLANT = root["PLANT"];
 const char* WORKCENTER = root["WORKCENTER"];
 Serial.println(COMPANY);
 Serial.println(PLANT);
 Serial.println(WORKCENTER);
 Screen1(COMPANY,LTD,PLANT,WORKCENTER);
}

void loop(){
//if(client.connected()){
//  do{
//    TopicName="RDR_00/";
//    id_Topic=TopicName+ MyId;
//    id_Topic.toCharArray(buff,100); // RDR_00/ID
//    client.publish( buff , "false");
//    Serial.println("false");
//    return;
//  }while(!readVcc());
//}
if(CurrentPage == 0){
  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
}
}
if(client.connected()){
    TopicName="WC_DETAILS/";
    id_Topic=TopicName+MyId;
    id_Topic.toCharArray(buff,100);
    client.subscribe(buff);//WC_DETAILS/MyId
    //Serial.println(buff);
        //client.unsubscribe(buff);//WC_DETAILS/MyId
    //Serial.println(buff);
    memset(buff,0,100); 
    TopicName="";
    id_Topic="";
  }
client.loop();
}
long readVcc(){
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

void Screen1(const char* COMPANY,const char* LTD,const char* PLANT,const char* WORKCENTER){
  CurrentPage=0;
  tft.fillScreen(White);
//  tft.setCursor (90,10);
//  tft.setTextSize (3);
//  tft.setTextColor(Black);
//  tft.println(COMPANY);
//  tft.println(PLANT);
//  tft.println(WORKCENTER);
  tft.setCursor (105,10);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.println("WELCOME TO");
  tft.setCursor (50,40);
  tft.println(COMPANY);
  tft.setCursor (90,70);
  tft.println(LTD);
  tft.setCursor (25,110);
  tft.print("PLANT  ");
  tft.setCursor(100,105);
  tft.setTextSize(4);
  tft.print("  :");
  tft.setCursor(170,110);
  tft.setTextSize(2);
  tft.println(PLANT);
  tft.setCursor (25,140);
  tft.print("WORKCENTER");
  tft.setCursor(145,135);
  tft.setTextSize(4);
  tft.print(":");
  tft.setTextSize(2);
  tft.setCursor(170,140);
  tft.println(WORKCENTER);
  tft.setCursor(10,190);
  tft.println("<---  SWIPE YOUR ID  --->");
  }
  
void initialize(){
  tft.fillScreen(White);
  tft.setCursor (90,1);
  tft.setTextSize (3);
  tft.setTextColor(Black);
  tft.print("INITIALIZING");
  for(i=0;i<3;i++){
     tft.print(".");
     }
     i=0;
}
