String topicName = String("iot-2/evt/status/fmt/json");
 String json;
 int tempF=0;
 int tempC=0;
 int humidity=0;
void setup(){
  Serial.begin(9600);
}
void loop(){
  char clientStr[34];
  topicName.toCharArray(clientStr,34);
  Serial.println(clientStr);
  Serial.println();
  Serial.println();
  Serial.println(topicName);
  json = buildJson();
  char jsonStr[200];
  json.toCharArray(jsonStr,200);
  Serial.println();
  Serial.println();
  Serial.println(json);
  Serial.println();
  Serial.println();
  Serial.println(jsonStr);
  Serial.println("----------------------------------------------");
  delay(50);
//  Serial.print("attempt to send ");
//  Serial.println(clientStr);
//  Serial.print("to ");
//  Serial.println(topicStr);
  
}
String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"myName\": \"Arduino DHT11\",";
  data+="\n";
  data+="\"temperature (F)\": ";
  data+=(int)tempF;
  data+= ",";
  data+="\n";
  data+="\"temperature (C)\": ";
  data+=(int)tempC;
  data+= ",";
  data+="\n";
  data+="\"humidity\": ";
  data+=(int)humidity;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}
