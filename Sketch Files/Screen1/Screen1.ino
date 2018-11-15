#include <ArduinoJson.h>
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
#include<EEPROM.h>
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
 const char* PLANT = root["PLANT"];
 const char* WORKCENTER = root["WORKCENTER"];
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



