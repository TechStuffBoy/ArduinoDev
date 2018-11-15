//defining input and output pins in arduino
int arduino_in_1=3;
int arduino_out_1=4;

// Adding required Libraries
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
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

void setup(){
  Ethernet.begin(mac,ip,gateway,subnet);
  pinMode(arduino_in_1, INPUT);
  pinMode(arduino_out_1,OUTPUT);
  Serial.begin(9600);
  Serial.println("Ethernet Begin");
}

void loop(){
   while(!client.connected()){
     Serial.println("Trying to connect client to server...");
     client.connect("My_ArduinoClient");
     if(client.connected()){
      Serial.println ("Success!!, Client Connected! ");
     }
   }

     if(client.connected()){
      Serial.println("Successfully Connected With Client");
      client.publish("MyTopic","Type in \"ControlTopic\"");
      if(digitalRead(arduino_in_1)== HIGH){
       client.publish("MyTopic","INPUTHIGH");
       Serial.println("Publishing Input as HIGH");
      }
      if(digitalRead(arduino_in_1)== LOW){
        client.publish("MyTopic","INPUTLOW");
        Serial.println("Publishing Input as LOW");
      }
      client.subscribe("ControlTopic");
      delay(500);
     }
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
  
  while(msgString.equals("RELAYON")){
     digitalWrite(arduino_out_1,HIGH);
     Serial.println("Relay is ON");
     break;
  }

   while(msgString.equals("RELAYOFF")){
     digitalWrite(arduino_out_1,LOW);
     Serial.println("Relay is OFF");
     break;
  }
    
 }

  


