#include<String.h>
String jsonSTRING;
float tempC=77.0;
float tempF=77.0;
char message_buff[150];
char jsonSTRING_buffer[200];

void setup(){
  Serial.begin(9600);
}

void loop(){
    jsonSTRING= "{\n\"Sensors\":{\n\"RDI000\":\"HIGH\"\n}\n}";
    jsonSTRING.toCharArray(jsonSTRING_buffer,200);
    Serial.println(jsonSTRING_buffer);
    memset(jsonSTRING_buffer,0,200);
    jsonSTRING=" ";
    
    jsonSTRING="{\n\"Sensors\":{\n\"RDI001\":(int)in_count\n}\n}";
    jsonSTRING.toCharArray(jsonSTRING_buffer,200);
    Serial.println(jsonSTRING_buffer);
    memset(jsonSTRING_buffer,0,200);
    jsonSTRING=" ";
    
    jsonSTRING=buildJson();
    jsonSTRING.toCharArray(jsonSTRING_buffer,200);
    Serial.println(jsonSTRING_buffer);
    memset(jsonSTRING_buffer,0,200);
    jsonSTRING=" ";
}

 String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
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
  data+="}";
  data+="\n";
  data+="}";
  return data;
}


