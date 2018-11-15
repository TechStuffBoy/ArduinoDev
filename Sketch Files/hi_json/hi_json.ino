#include <ArduinoJson.h>
void setup() {
  Serial.begin(9600);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root1 = jsonBuffer.createObject();
  JsonObject& root2 = jsonBuffer.createObject();
  JsonObject& root3 = jsonBuffer.createObject();
  
  root1["sensor"] = "gps";
  root1["time"] = 1351824120;
  JsonArray& data = root1.createNestedArray("data");
  data.add(48.756080, 6);
  data.add(2.302038, 6);
  root1.printTo(Serial);
  Serial.println();
  root2["sensor"] = "gps";
  root2["time"] = 1351824120;
  JsonArray& data1 = root2.createNestedArray("data");
  data1.add(48.756080, 6);
  data1.add(2.302038, 6);
  root2.printTo(Serial); 
  Serial.println();
  root3["sensor"] = "gps";
  root3["time"] = 1351824120;
  JsonArray& data2 = root3.createNestedArray("data");
  data2.add(48.756080, 6);
  data2.add(2.302038, 6);
  root3.printTo(Serial); 
}

void loop() {
  
}
