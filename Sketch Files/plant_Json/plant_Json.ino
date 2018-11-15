/*
<------ ARDUINO INPUTS -------->
1)MainSwitch   == RDI000
2)Proximity_1  == RDI001
3)TempSensor   == RAI000
4)Proximity_2  == RDI002
5)Camera_reject == RDI003

<------ ARDUINO OUTPUTS ------->
1)ConveyorBelt == RDO000
2)CrankShaft   == RDOOO1
3)Solenoid     == RDO002
*/

#include <ArduinoJson.h>
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>


char message_buff[200];
StaticJsonBuffer<200> jsonBuffer;

String jsonStr;
char jsonStr_buff[200];

 byte RDI_000 = 2; // MainSwitch
 byte RDI_001 = 3; // Proximity_1
 byte RAI_000 = A0; // TempSensor
 byte RDI_002 = 5; // Proximity_2
 byte RDI_003 = 6; //Camera_input

byte RDO_000 = 7; //ConveyorBelt
byte RDO_001 = 8; //CrankShaft
byte RDO_002 = 9; //Solenoid

int RDI_000_val=0; //MainSwitch
int RDI_001_val=0;
int RAI_000_val=0;
int RDI_002_val=0;
int RDI_003_val=0;
float volts=0;
float tempC=0;
float tempF=0;
int in_count=0;
int bug_count=0;





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

void setup(){
  //Starting Serial Communication
  Serial.begin(9600);
  
  // Starting Ethernet
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.println("Ethernet Begin");

   // Initializing pins as INPUT
  pinMode( RDI_000 , INPUT );
  pinMode( RDI_001 , INPUT );
  pinMode( RDI_002 , INPUT );
  pinMode( RDI_003 , INPUT );
  
  
  // Initializing pins as OUTPUT
  pinMode( RDO_000 , OUTPUT );
  pinMode( RDO_001 , OUTPUT );
  pinMode( RDO_002 , OUTPUT );
  
}

 void loop(){
  while(!client.connected()){
     Serial.println("Trying to connect client to server...");
     client.connect("Arduino_1");
     if(client.connected()){
      Serial.println ("Success!!, Client Connected! ");
     }
   }
if(client.connected()){
      
      RDI_000_val=digitalRead(RDI_000);
      jsonStr = "{\"MainSwitch\":\"HIGH\"}";
      jsonStr.toCharArray(jsonStr_buff, 200);
//       if(RDI_000_val == HIGH ){
//        client.publish("A_1/RDI_000",jsonStr_buff);
//      }
      client.publish("A_1/RDI_001","SAP ON");
      memset(jsonStr_buff,0,200);
      jsonStr=" ";
      client.subscribe("BB/A_1/RDI_000");

      
  } 
  delay(500);
  client.loop();
 }
void callback(char* topic,byte* payload,unsigned int length){
  int i=0;
  for(i=0;i<length;i++){
    message_buff[i]=payload[i];
  }
  // Hav to stop the string so giving \0
  message_buff[i]='\0';
  Serial.println(message_buff);

  JsonObject& MainControl= jsonBuffer.parseObject(message_buff);
  boolean ConBelt = MainControl["RDO_000"];
  boolean CrankShaft = MainControl["RDO_001"];

  Serial.println(ConBelt);
  Serial.println(CrankShaft);
  if(ConBelt){
    
   // client.publish("A_1/RDI_001","CRANK ON");
    Serial.println("ConBelt ON");
    digitalWrite( RDO_000, HIGH );
   }
  if(ConBelt == 0){
    
    Serial.println("ConBelt OFF");
    digitalWrite( RDO_000, LOW );
   } 
   
  if(CrankShaft){
    
    Serial.println("CrankShaft ON");
    digitalWrite( RDO_001, HIGH );
}
 if(CrankShaft == 0){
 
    Serial.println("CrankShaft OFF");
    digitalWrite( RDO_001, LOW );
 }

}
