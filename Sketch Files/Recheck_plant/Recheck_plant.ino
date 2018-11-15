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
#include<String.h>
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
//char jsonHIGH_buffer[200];
String jsonSTRING;
char message_buff[150];
char jsonSTRING_buffer[200];


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
byte RAI000 = A0; // TempSensor
byte RDI002 = 5; // Proximity_2
byte RDI003 = 6; //Camera_input

byte RDO000 = 7; //ConveyorBelt
byte RDO001 = 8; //CrankShaft
byte RDO002 = 9; //Solenoid

int RDI000_val=0; //MainSwitch
int RDI001_val=0;
int RAI000_val=0;
int RDI002_val=0;
int RDI003_val=0;
float volts=0;
float tempC=0;
float tempF=0;
int in_count=0;
int bug_count=0;


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
     Serial.println("Trying to connect client to server...");
     client.connect("Arduino_1");
     if(client.connected()){
      Serial.println ("Success!!, Client Connected! ");
     }
   }
 
 if(client.connected()){
      Serial.println("Successfully Connected With Client");
      RDI000_val=digitalRead(RDI000);
      jsonSTRING= "{\n\"Sensors\":{\n\"RDI000\":\"HIGH\"\n}\n}";
      //char jsonSTRING_buffer[200];
      jsonSTRING.toCharArray(jsonSTRING_buffer,200);
      if(RDI000_val == HIGH ){
        client.publish("/Arduino_1/RDI000" ,jsonSTRING_buffer);
      }
      memset(jsonSTRING_buffer,0,200);
      jsonSTRING=" ";
      client.subscribe("BeagleBone/Arduino_1/RDI000"); // MainSwitch
      // Hence now CrankShaft and ConveyorBelt is ON( Go to CallBack to Check)
        check_proximity_1();
        check_Temp();
        client.subscribe("BeagleBone/Arduino_1/RAI000"); //Temp
        check_rejection();
        client.subscribe("BeagleBone/Arduino_1/RDI003"); //for Solenoid
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

   while(msgString.equals("{\n\"order\":{\n\"RDO000\":\"HIGH\",\n\"RDO001\":\"HIGH\"\n}\n}")){
      // Here put CrankShaft and ConveyorBelt High
      break;
  }

   while(msgString.equals("{\n\"order\":{\n\"RDO000\":\"LOW\",\n\"RDO001\":\"LOW\"\n}\n}")){
      // Here put CrankShaft and ConveyorBelt LOW
      //Immediately stop the plant if temp is high or low than preset
      break;
  }

   while(msgString.equals("{\n\"order\":{\n\"RDO002\":\"HIGH\"\n}\n}")){
      // Here set Solenoid as High
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
    jsonSTRING="{\n\"Sensors\":{\n\"RDI001\":(int)in_count\n}\n}";
    jsonSTRING.toCharArray(jsonSTRING_buffer,200);
    
    client.publish("/Arduino_1/RDI001",jsonSTRING_buffer);
    
    memset(jsonSTRING_buffer,0,200);
    jsonSTRING=" ";
    break;
  }
}
} 

void check_Temp(){
  RAI000_val=analogRead(RAI000);
  volts=RAI000_val/205.0;
  tempC=100*volts-50;
  //tempF=tempC*(9.0/5.0)+32.0;
  jsonSTRING="{\n\"Sensors\":{\"RAI000\":(float)tempC\n}\n}";
  jsonSTRING.toCharArray(jsonSTRING_buffer,200);
  client.publish("/Arduino_1/RAI000",jsonSTRING_buffer);
  memset(jsonSTRING_buffer,0,200);
  jsonSTRING=" ";
}

void check_rejection(){
   RDI003_val=digitalRead(RDI003);
   if(RDI003_val == HIGH ){

    // Checking Bug sensor Value
 RDI002_val= digitalRead(RDI002);
 // If bug sensor is HIGH ==> object Found
  while(RDI002_val != 0 )
 {
 // We have to count that Tile as Wrong one
   RDI002_val= digitalRead(RDI002);
   while(!RDI002_val)
  {
    
    // Increase the bug_Counter
    bug_count++;
    jsonSTRING="{\n\"Sensors\":{\n\"RDI002\":(int)bug_count\n}\n}";
    jsonSTRING.toCharArray(jsonSTRING_buffer,200);
    client.publish("/Arduino_1/RDI002",jsonSTRING_buffer);
    memset(jsonSTRING_buffer,0,200);
    jsonSTRING=" ";
    break;
}
}
   }
}
