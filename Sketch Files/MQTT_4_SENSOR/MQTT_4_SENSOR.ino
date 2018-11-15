//Including necessary libraries.
#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>

// Assigning analog input pins
int proximity_1=2;
int proximity_2=3;
int temp_sensor=A0;
int moisture_sensor=A1;

//assigning output pins
int proximity_1_trigger=4;
int proximity_2_trigger=5;
int temp_sensor_trigger=6;
int moisture_sensor_trigger=7;

//assigning variables to store values
int proximity_1_val=0;
int proximity_2_val=0;
int Temp_Sense=0;
float volts=0;
float tempF=0;
float tempC=0;
int moisture_sensor_val=0;
int moisture_percent=0;

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

void setup()
{
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.begin(9600);
  Serial.println("Ethernet Begin ");
  pinMode(proximity_1,INPUT);
  pinMode(proximity_2,INPUT);
  pinMode(proximity_1_trigger,OUTPUT);
  pinMode(proximity_2_trigger,OUTPUT);
  pinMode(temp_sensor_trigger,OUTPUT);
  pinMode(moisture_sensor_trigger,OUTPUT);
}

void loop(){
  readTempSen();
  readProximity_1();
  readProximity_2();
  readMoistureSen();

   while(!client.connected()){
    Serial.println("Trying to connect client with server");
    client.connect("My_arduino_Temp_sensor");
    if(client.connected()){
      Serial.println("Success!!,Client Connected!");
    }
   }
    if(client.connected()){
     String json=buildJson();
     char jsonStr[300];
     json.toCharArray(jsonStr,300);
     // We hav to convert string to char array,then only we can pass this value to client.publish()
     boolean resultSend= client.publish("/arduino/4Sensors", jsonStr);
     if(resultSend)
      Serial.println("Successfully Sent");
     else
      Serial.println("Warning!!,Sent Unsuccessfull");
      delay(3000);
  }
  client.loop();
}
}

void readTempSen(){
  Temp_Sense=analogRead(temp_sensor);
  volts=Temp_Sense/205.0;
  tempC=100*volts-50;
  tempF=tempC*(9.0/5.0)+32.0;
  Serial.print("tempC :");
  Serial.print(tempC);
  Serial.print(" , ");
  Serial.print("tempF :");
  Serial.println(tempF);
}

void readProximity_1(){
  proximity_1_val=digitalRead(proximity_1);
  Serial.print("proximity_1 :");
  Serial.println(proximity_1_val);
}

void readProximity_2(){
  proximity_1_val=digitalRead(proximity_2);
  Serial.print("proximity_2 :");
  Serial.println(proximity_2_val);
}
  
void readMoistureSen(){
  moisture_sensor_val=analogRead(moisture_sensor);
  moisture_percent=map(moisture_percent,1023,465,0,100);
  Serial.print("moisture_sensor_percent :");
  Serial.print(moisture_percent);
  Serial.println("%");
}

String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"TilesUnit\": {";
  data+="\n";
  data+="\"myName\": \"Arduino TMP36\",";
  data+="\n";
  data+="\"temperature (F)\": ";
  data+=(int)tempF;
  data+= ",";
  data+="\n";
  data+="\"temperature (C)\": ";
  data+=(int)tempC;
  data+="\n";
  data+="\"Proximity_1\": ";
  data+= TRUE;
  data+= ",";
  data+="\n";
  data+="\"Proximity_2\": ";
  data+=(int)proximity_2_val;
  data+= ",";
  data+="\n";
  data+="\"MyName\": \"Moisture_sen\",";
  data+="\n";
  data+="\"Moisture_%\": ";
  data+=(int)moisture_percent;
  data+= ",";
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}

void callback(char* topic,byte* payload,unsigned int length){
  }

