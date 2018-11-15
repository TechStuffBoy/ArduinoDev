#include <ArduinoJson.h>
#include <String.h>

void setup() {
  Serial.begin(9600);
  StaticJsonBuffer<350> jsonBuffer;
  String json= "{\n\"COMPANY\":\n{\n\"NAME\":\"Murudeswar Ceramics\",\n\"ID\":1111\n},\n\"PLANT\":\n{\n\"NAME\":\"DUMKUR\",\n\"ID\":123\n},\n\"WORK_CENTER_1\":\n{\n\"NAME\":\"<WC_NAME>\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n},\n\"WORK_CENTER_2\":\n{\n\"NAME\":\"<WC_NAME>\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n}\n}";
  Serial.println(json);
  char json1[]= "{\n\"COMPANY\":\n{\n\"NAME\":\"Murudeswar Ceramics\",\n\"ID\":1111\n},\n\"PLANT\":\n{\n\"NAME\":\"DUMKUR\",\n\"ID\":123\n},\n\"WORK_CENTER_1\":\n{\n\"NAME\":\"<WC_NAME>\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n},\n\"WORK_CENTER_2\":\n{\n\"NAME\":\"<WC_NAME>\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n}\n}";
  JsonObject& root = jsonBuffer.parseObject(json1);
  if (!root.success()){
    Serial.println("parseObject() failed");
    return;
  }

  const char* CompanyName = root["COMPANY"]["NAME"];
  int CompanyID = root["COMPANY"]["ID"];
  const char* PlantName = root["PLANT"]["NAME"];
  int PlantID = root["PLANT"]["ID"];
  const char* WorkCenterName_1 = root["WORK_CENTER_1"]["NAME"];
  int WorkCenterCode_1 = root["WORK_CENTER_1"]["CODE"];
  int WorkCenterID_1 = root["WORK_CENTER_1"]["ID"];
  int WC_1_RDI_0 = root["WORK_CENTER_1"]["RDI_0"];
  int WC_1_RDO_0 = root["WORK_CENTER_1"]["RDO_0"];
  int WC_1_RAI_0 = root["WORK_CENTER_1"]["RAI_0"];
  const char* WorkCenterName_2 = root["WORK_CENTER_2"]["NAME"];
  int WorkCenterCode_2 = root["WORK_CENTER_2"]["CODE"];
  int WorkCenterID_2 = root["WORK_CENTER_2"]["ID"];
  int WC_2_RDI_0 = root["WORK_CENTER_2"]["RDI_0"];
  int WC_2_RDO_0 = root["WORK_CENTER_2"]["RDO_0"];
  int WC_2_RAI_0 = root["WORK_CENTER_2"]["RAI_0"];

    Serial.print(F("CompanyName :"));
    Serial.println(CompanyName);
    Serial.print(F("CompanyID :"));
    Serial.println(CompanyID);
    Serial.print(F("PlantName :"));
    Serial.println(PlantName);
    Serial.print(F("PlantID :"));
    Serial.println(PlantID);
    Serial.print(F("WorkCenterName_1 :"));
    Serial.println(WorkCenterName_1);
    Serial.print(F("WorkCenterCode_1 :"));
    Serial.println(WorkCenterCode_1);
    Serial.print(F("WorkCenterID_1 :"));
    Serial.println(WorkCenterID_1);
    Serial.print(F("WC_1_RDI_0 :"));
    Serial.println(WC_1_RDI_0);
    Serial.print(F("WC_1_RDO_0 :"));
    Serial.println(WC_1_RDO_0);
    Serial.print(F("WC_1_RAI_0 :"));
    Serial.println(WC_1_RAI_0);
    Serial.print(F("WorkCenterName_2 :"));
    Serial.println(WorkCenterName_2);
    Serial.print(F("WorkCenterCode_2 :"));
    Serial.println(WorkCenterCode_2);
    Serial.print(F("WorkCenterID_2 :"));
    Serial.println(WorkCenterID_2);
    Serial.print(F("WC_2_RDI_0 :"));
    Serial.println(WC_2_RDI_0);
    Serial.print(F("WC_2_RDO_0 :"));
    Serial.println(WC_2_RDO_0);
    Serial.print(F("WC_2_RAI_0 :"));
    Serial.println(WC_2_RAI_0);

 }

void loop(){
  
}

