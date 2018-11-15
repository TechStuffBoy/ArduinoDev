#include <MFRC522.h>

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned int interval = 5000;

boolean rfidFlag = false;

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
 byte server[]={192,168,3,14};

//My Arduino's IP address
  byte ip[]={192,168,3,34};
  byte subnet[]={255,255,255,0};
  byte gateway[]={192,168,3,1};
  
 //Initializing the Client 
 EthernetClient ethClient;
 
 //Creating the PubSubClient Object
 PubSubClient client(server,1883,callback,ethClient);
 
void setup()
 
{ 
  SPI.begin();
  pinMode(23 , OUTPUT); // ss for rfid
  pinMode(10 , OUTPUT); // ss for ethernet
  pinMode(ledPin,OUTPUT);
  digitalWrite(23 , HIGH);
  digitalWrite(10 , LOW);
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.begin(9600);
  Serial.println("Ethernet Begin");
}

void loop()
{
  currentMillis = millis();
  
  if( rfidFlag == true ){
    digitalWrite(10 , HIGH);
    digitalWrite(23 , LOW);
    if( currentMillis - previousMillis >= interval){
      Serial.println("I am in loop");
       digitalWrite(10 , LOW);
       digitalWrite(23 , HIGH);
       rfidFlag = false;
       
    }
  }
  
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
    client.publish("MyTopic"," Type ON or OFF in PUBLISHER LD001");
    client.subscribe("L");
    
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


  //Serial.println(message_buff);

 String mess = message_buff;
 Serial.println(mess);
 if( mess == "connect"){
  Serial.println("Connect is published");
   previousMillis = currentMillis;
   rfidFlag = true ;
  }
//  String msgString=String(message_buff);
//  Serial.println("PayLoad: " +msgString);
//
//  while(msgString.equals("ON")){
//    digitalWrite(ledPin,HIGH);
//    client.publish("MyTopic","Success!,Your Light is Glowing");
//    Serial.println("published Success!,Your Light is Glowing in Subscriberin Mqtt lens");
//    //delay(100);
//    break;
//     }
//    while(msgString.equals("OFF")){
//      digitalWrite(ledPin,LOW);
//      client.publish("MyTopic","Warning! Light is OFF");
//      Serial.println("Published Warning! Light is OFF in Subscriber in MqttLens");
//      //delay(100);
//      break;
//    }
}

