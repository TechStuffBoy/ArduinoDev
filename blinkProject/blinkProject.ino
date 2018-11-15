#include <SPI.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include <PubSubClient.h>
#include <SPFD5408_TouchScreen.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_Adafruit_GFX.h>
#include <registers.h>
#include <pin_magic.h>
#include <gfxfont.h>

#define LED_PIN 13


/*
<------ ARDUINO INPUTS -------->
1)MainSwitch   == RDI000
2)Proximity_1  == RDI001
3)TempSensor   == RDI002
4)Proximity_2  == RDI003

<------ ARDUINO OUTPUTS ------->
1)ConveyorBelt == RDO000
2)CrankShaft   == RDOOO1
3)Solenoid     == RDO002

*/
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
//char jsonHIGH_buffer[200];
String jsonHIGH= "{ \n \"RDI000\":{\n \"value\":\"HIGH\"\n}\n}";
String jsonPROX_1;
char message_buff[100];


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


byte RDI000 = 2; // MainSwitch
byte RDI001 = 3; // Proximity_1
byte RDI002 = 4; // TempSensor
byte RDI003 = 5; // Proximity_2

byte RDO000 = 6; //ConveyorBelt
byte RDO001 = 7; //CrankShaft
byte RDO002 = 8; //Solenoid

int RDI000_val=0; //MainSwitch
int RDI001_val=0;
int RDI002_val=0;
int RDI003_val=0;
float volts=0;
float tempC=0;
float tempF=0;
int in_count=0;


void setup(){
  //Starting Serial Communication
  Serial.begin(9600);
  
  // Initializing pins as INPUT
  pinMode( RDI000 , INPUT );
  pinMode( RDI001 , INPUT );
  pinMode( RDI002 , INPUT );
  pinMode( RDI003 , INPUT );
  
  // Initializing pins as OUTPUT
  pinMode( RDO000 , OUTPUT );
  pinMode( RDO001 , OUTPUT );
  pinMode( RDO002 , OUTPUT );

  //Starting Ethernet
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.println("Ethernet Begin");
}

void loop(){
  while(!client.connected()){
    Serial.println(F("Trying to connect client to server..."));
    client.connect("Arduino_1");
    if(client.connected()){
      Serial.println ("Success!!, Client Connected! ");
    }
  }
  
  if(client.connected()){
    Serial.println(F("Successfully Connected With Client"));
    RDI000_val=digitalRead(RDI000);
    char jsonHIGH_buffer[100];
    jsonHIGH.toCharArray(jsonHIGH_buffer,100);
    if(RDI000_val == HIGH ){
      client.publish("/Arduino_1/RDI000" ,jsonHIGH_buffer);
    }
    client.subscribe("BeagleBone/Arduino_1/RDI000");
    // Hence now CrankShaft and ConveyorBelt is ON( Go to CallBack to Check)
    check_proximity_1();
    check_Temp();
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
  Serial.println("PayLoad: " +msgString);

  while(msgString.equals("{\n\"RDI000\":{\n\"ConveyorBelt\":\"HIGH\",\n\"CrankShaft\":\"HIGH\"\n}\n}")){
  // Here put CrankShaft and ConveyorBelt High
  break;
}
}

void check_proximity_1(){
// Reading the in_sensor  
RDI001_val=digitalRead(RDI001);
// Function for InSensor
while(RDI001_val != 0)
{
  RDI001_val=digitalRead(RDI001);
  while(!RDI001_val)
  {
    // Increase the in_Counter
    in_count++;
    char jsonPROX_1_buffer[100];
    jsonPROX_1="{\n\"RDI001\":{\n\"in_count\":(int)in_count\n}\n}";
    jsonPROX_1.toCharArray(jsonPROX_1_buffer,100);
    client.publish("/Arduino_1/RDI001",jsonPROX_1_buffer);
    break;
  }
} 
}

void check_Temp(){
  RDI002_val=analogRead(RDI002);
  volts=RDI002_val/205.0;
  tempC=100*volts-50;
  tempF=tempC*(9.0/5.0)+32.0;
  String jsonTemp="";
}