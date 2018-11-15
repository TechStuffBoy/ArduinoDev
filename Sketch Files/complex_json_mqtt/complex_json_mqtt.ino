#include <ArduinoJson.h>

/*Start Mosquitto Server in PC and Open Client and Subcribe to MyTopic 
 * and Publish "ON" or "OFF" in Topic "LD001" to turn Led On and OFF 
 */


 //Includeing Necessary Libraries
 
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
int ledPin=3;
char message_buff[400];

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
     client.connect("Arduino1");
     if(client.connected()){
      Serial.println ("Success!!, Client Connected! ");
     }
  }
  if(client.connected()){
    //Serial.println("Please type \"ON\" or \"OFF\" through PUBLISHER to turn LED ON or OFF");
    //publish is not necessary,it is just for ack purpose.
    //client.publish("MyTopic"," Type ON or OFF in PUBLISHER LD001");
    client.subscribe("LD001");
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

 StaticJsonBuffer<400> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject(message_buff);

 
  const char* CompanyName = root["COMPANY"]["NAME"];
  int CompanyID = root["COMPANY"]["ID"];
  const char* PlantName = root["PLANT"]["NAME"];
  int PlantID = root["PLANT"]["ID"];
  const char* WorkCenterName_1 = root["WORK_CENTER_1"]["NAME"];
  int WorkCenterCode_1 = root["WORK_CENTER_1"]["CODE"];
  int WorkCenterID_1 = root["WORK_CENTER_1"]["ID"];
  int WC_1_RDI_0 = root["WORK_CENTER_1"]["RDI_0"];
  int WC_1_RDO_0 = root["WORK_CENTER_1"]["RDO_0"];
  int WC_1_RAI_0 = root["WORK_CENTER_1"]["RAI_0"];
  const char* WorkCenterName_2 = root["WORK_CENTER_2"]["NAME"];
  int WorkCenterCode_2 = root["WORK_CENTER_2"]["CODE"];
  int WorkCenterID_2 = root["WORK_CENTER_2"]["ID"];
  int WC_2_RDI_0 = root["WORK_CENTER_2"]["RDI_0"];
  int WC_2_RDO_0 = root["WORK_CENTER_2"]["RDO_0"];
  int WC_2_RAI_0 = root["WORK_CENTER_2"]["RAI_0"];

 
    Serial.print(F("CompanyName :"));
    Serial.println(CompanyName);
    Serial.print(F("CompanyID :"));
    Serial.println(CompanyID);
    Serial.print(F("PlantName :"));
    Serial.println(PlantName);
    Serial.print(F("PlantID :"));
    Serial.println(PlantID);
    Serial.print(F("WorkCenterName_1 :"));
    Serial.println(WorkCenterName_1);
    Serial.print(F("WorkCenterCode_1 :"));
    Serial.println(WorkCenterCode_1);
    Serial.print(F("WorkCenterID_1 :"));
    Serial.println(WorkCenterID_1);
    Serial.print(F("WC_1_RDI_0 :"));
    Serial.println(WC_1_RDI_0);
    Serial.print(F("WC_1_RDO_0 :"));
    Serial.println(WC_1_RDO_0);
    Serial.print(F("WC_1_RAI_0 :"));
    Serial.println(WC_1_RAI_0);
    Serial.print(F("WorkCenterName_2 :"));
    Serial.println(WorkCenterName_2);
    Serial.print(F("WorkCenterCode_2 :"));
    Serial.println(WorkCenterCode_2);
    Serial.print(F("WorkCenterID_2 :"));
    Serial.println(WorkCenterID_2);
    Serial.print(F("WC_2_RDI_0 :"));
    Serial.println(WC_2_RDI_0);
    Serial.print(F("WC_2_RDO_0 :"));
    Serial.println(WC_2_RDO_0);
    Serial.print(F("WC_2_RAI_0 :"));
    Serial.println(WC_2_RAI_0);

    }


