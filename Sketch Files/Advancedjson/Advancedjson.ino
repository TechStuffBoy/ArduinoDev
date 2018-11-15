#include <ArduinoJson.h>
#include <string.h>




void setup() {
  Serial.begin(9600);
  StaticJsonBuffer<200> jsonBuffer;
  char json[] = "{\"data\":{\"time\":{\"day\":1,\"month\":3,\"year\":16,\"hours\":9,\"mins\":59,\"secs\":14}}}";
  JsonObject& root = jsonBuffer.parseObject(json);
   if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

    int day     = root["data"]["time"]["day"];
    int month   = root["data"]["time"]["month"];
    int year    = root["data"]["time"]["year"];
    int hours   = root["data"]["time"]["hours"];
    int mins    = root["data"]["time"]["mins"];
    int secs    = root["data"]["time"]["secs"];
    Serial.print("Day:");
    Serial.println(day);
    Serial.print("Month:");
    Serial.println(month);
    Serial.print("Year:");
    Serial.println(year);
    Serial.print("Hours:");
    Serial.println(hours);
    Serial.print("Mins:");
    Serial.println(mins);
    Serial.print("Secs:");
    Serial.println(secs);
}
void loop(){
}


