#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
String myString;
boolean hello =false;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED
};

byte server[]={192,168,3,14};
byte ip[]={192,168,3,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,3,1};

EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);

void setup()
{
Ethernet.begin(mac,ip,gateway,subnet);
Serial.begin(9600);
Serial.println("Ethernet Begin");

}
void loop()
{
  while(!client.connected()){
    Serial.println("Trying to connect client to server...");
    client.connect("My_arduinoClient");
    if(client.connected()){ 
      Serial.println("Client Connected!!");
    }
  }
 
  if(client.connected()){
    if( hello == false ){
      client.publish("hi","hello,World");
      hello = true;
     }
     client.subscribe("inTopic");
  }
    
    client.loop();
}
  
 void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Length :");
  Serial.println(length);
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    }
  Serial.println();
}


  

  
