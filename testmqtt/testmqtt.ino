#include <PubSubClient.h>
#include <SPI.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
/*Start Mosquitto Server in PC and Open Client and Subcribe to MyTopic 
 * and Publish "ON" or "OFF" in Topic "LD001" to turn Led On and OFF 
 */

//Includeing Necessary Libraries
//#include<SPI.h>
//#include<Ethernet.h>
//#include<PubSubClient.h>
int ledPin=3;
char message_buff[100];

// Including My Arduino Ethernet Shield Mac Address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED
};

/*Initializing the Mosquitto installed PC's
 IP address wgich is the Broker*/
 byte server[]={192,168,3,13};

//My Arduino's IP address
  byte ip[]={192,168,3,50};
  byte subnet[]={255,255,255,0};
  byte gateway[]={192,168,3,1};
  
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
    client.publish("MyTopic"," Type ON or OFF in PUBLISHER LD001");
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
  Serial.print("Topic :" );
  Serial.println(topic);
  Serial.println("PayLoad: " +msgString);

  while(msgString.equals("ON")){
    digitalWrite(ledPin,HIGH);
    client.publish("MyTopic","Success!,Your Light is Glowing");
    Serial.println("published Success!,Your Light is Glowing in Subscriberin Mqtt lens");
    //delay(100);
    break;
     }
    while(msgString.equals("OFF")){
      digitalWrite(ledPin,LOW);
      client.publish("MyTopic","Warning! Light is OFF");
      Serial.println("Published Warning! Light is OFF in Subscriber in MqttLens");
      //delay(100);
      break;
    }
}

