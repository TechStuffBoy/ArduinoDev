#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>
String data;
 float voltage, degreesC, degreesF;
 int senPin=0;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xFF, 0x06, 0xED
};

byte server[]={192,168,1,14};

//My Arduino's IP address
byte ip[]={192,168,1,34};
byte subnet[]={255,255,255,0};
byte gateway[]={192,168,1,1};

EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);

void callback(char* topic,byte* payload,unsigned int length){
  
}
void setup()
{
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.begin(9600);
  Serial.println("Ethernet Begin ");
}
void loop()
{
  readTempSen();
  while(!client.connected()){
    Serial.println("Trying to connect client with server");
    client.connect("My_arduino_Temp_sensor");
    if(client.connected()){
      Serial.println("Success!!,Client Connected!");
    }
   }
    
    if(client.connected()){
     String json=buildJson();
     char jsonStr[200];
     json.toCharArray(jsonStr,200);
     // We hav to convert string to char array,then only we can pass this value to client.publish()
     boolean resultSend= client.publish("/arduino/Temp", jsonStr);
     if(resultSend)
      Serial.println("Successfully Sent");
     else
      Serial.println("Warning!!,Sent Unsuccessfull");
      delay(3000);
  }
  client.loop();
}
void readTempSen(){
 voltage=analogRead(senPin);
 degreesC = (voltage - 0.5) * 100.0;
 degreesF = degreesC * (9.0/5.0) + 32.0;
  }

  String buildJson() {
  data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"temperature (F)\": ";
  data+=(int)degreesF;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}

