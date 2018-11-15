#include<String.h>
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
//char jsonHIGH_buffer[200];
String jsonSTRING;
char message_buff[200];
char jsonSTRING_buffer[200];


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


byte RDI000 = 2; // ip1
byte RDI001 = 3; //ip2

byte RDO000 = 7; //op1
byte RDO001 = 8; //op2

int RDI000_va1l=0;
int RDI001_val2=0;

void setup(){
  //Starting Serial Communication
  Serial.begin(9600);
  
  // Initializing pins as INPUT
  pinMode( RDI000 , INPUT );
  pinMode( RDI001 , INPUT );

   // Initializing pins as OUTPUT
  pinMode( RDO000 , OUTPUT );
  pinMode( RDO001 , OUTPUT );

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
      //Serial.println("Successfully Connected With Client");
      RDI000_va1l=1;
      jsonSTRING= "{\n\"Sensors\":{\n\"RDI000\":\"HIGH\"\n}\n}";
      jsonSTRING.toCharArray(jsonSTRING_buffer,200);
      if(RDI000_va1l == 1 ){
        client.publish("/Arduino_1/RDI000" ,jsonSTRING_buffer);
      }
      memset(jsonSTRING_buffer,0,200);
      jsonSTRING=" ";
      client.subscribe("BeagleBone/Arduino_1/RDI000");

      RDI001_val2=0;
      jsonSTRING= "{\n\"Sensors\":{\n\"RDI001\":\"LOW\"\n}\n}";
      jsonSTRING.toCharArray(jsonSTRING_buffer,200);
      if(RDI001_val2 == 0 ){
        client.publish("/Arduino_1/RDI001" ,jsonSTRING_buffer);
      }
      memset(jsonSTRING_buffer,0,200);
      jsonSTRING=" ";
      client.subscribe("BeagleBone/Arduino_1/RDI001");
      
    }
    delay(1500);
    client.loop();
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

   while(msgString.equals("low")){
      // Here put CrankShaft and ConveyorBelt High
      Serial.println("CrankShaft and ConveyorBelt High");
      break;
  }
  while(msgString.equals("{\n\"order\":{\n\"RDO001\":\"LOW\",\n\"RDO001\":\"LOW\"\n}\n}")){
      // Here put CrankShaft and ConveyorBelt low
      Serial.println("CrankShaft and ConveyorBelt low");
      break;
  }
 }
