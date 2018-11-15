#include <ArduinoJson.h>

/*Start Mosquitto Server in PC and Open Client and Subcribe to MyTopic 
 * and Publish "ON" or "OFF" in Topic "LD001" to turn Led On and OFF 
 */


 //Includeing Necessary Libraries
 
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
int ledPin=3;
char message_buff[100];

// Including My Arduino Ethernet Shield Mac Address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED
};

/*Initializing the Mosquitto installed PC's
 IP address wgich is the Broker*/
 byte server[]={192,168,1,14};

//My Arduino's IP address
  byte ip[]={192,168,1,34};
  byte subnet[]={255,255,255,0};
  byte gateway[]={192,168,1,1};
  
 //Initializing the Client 
 EthernetClient ethClient;
 
 //Creating the PubSubClient Object
 PubSubClient client(server,1883,callback,ethClient);
 
void setup()
 
{
  pinMode(ledPin,OUTPUT);
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.begin(9600);
  Serial.println("Ethernet Begin");
}

void loop()
{
  while(!client.connected()){
     Serial.println("Trying to connect client to server...");
     client.connect("My_ArduinoClient");
     if(client.connected()){
      Serial.println ("Success!!, Client Connected! ");
     }
  }
  if(client.connected()){
    //Serial.println("Please type \"ON\" or \"OFF\" through PUBLISHER to turn LED ON or OFF");
    //publish is not necessary,it is just for ack purpose.
   //client.publish("MyTopic"," Type ON or OFF in PUBLISHER LD001");
    client.subscribe("Hello");
    delay(1000);
  }
  client.loop();
}
// This Function Handles the Data send to Subscribed Topic

void callback(char* topic,byte* payload,unsigned int length){
  int i=0;
  for(i=0;i<length;i++){
    message_buff[i]=payload[i];
  }
  // Hav to stop the string so giving \0
  message_buff[i]='\0';

  String msgString=String(message_buff);
 // Serial.println("PayLoad: " +msgString);
 Serial.println(message_buff);

 StaticJsonBuffer<300> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject(message_buff);

 const char* COMPANY = root["COMPANY"];
 const char* PLANT = root["PLANT"];
 boolean WORKCENTER = root["WORKCENTER"];
 //{"COMPANY" :"Murdeswar ceramics","PLANT" :"TUMKUR","WORKCENTER";true}
   
Serial.println(COMPANY);
Serial.println(PLANT);
Serial.println(WORKCENTER);
    }


