char json[] = "{\n\"RUID\" :\n{\n\"ID\" : \"RU_01\",\n\"MACID\" : \"00:00:00:00:00:00\"\n},\n\"COMPANY\" :\n{\n\"CODE\" : \"MUCR\",\n\"DESC\" : \"Murdeshwar Ceramics\"\n},\n\"PLANT\" : \n{\n\"CODE\" : \"MUP1\",\n\"DESC\" : \"Plant1\"\n},\n\"WORKCENTER\" :\n{\n\"1\" :\n{\n\"CODE\" : \"Mixture\",\n\"DESC\" : \"Workcenter1\"\n},\n\"2\" : \n{\n\"CODE\" : \"Mixture\",\n\"DESC\" : \"Workcenter2\"\n},\n\"3\" :\n{\"CODE\" : \"Mixture\",\n\"DESC\" : \"Workcenter3\"\n}\n}\n}";
String json1[] = "{\n\"RUID\" :\n{\n\"ID1\" : \"RU_01\",\n\"ID2\" : \"RU_02\",\"MACID\" : \"00:00:00:00:00:00\"\n},\n\"COMPANY\" :\n{\n\"CODE\" : \"MUCR\",\n\"DESC\" : \"Murdeshwar Ceramics\"\n},\n\"PLANT\" : \n{\n\"CODE\" : \"MUP1\",\n\"DESC\" : \"Plant1\"\n},\n\"WORKCENTER\" :\n{\n\"1\" :\n{\n\"CODE\" : \"Mixture\",\n\"DESC\" : \"Workcenter1\"\n},\n\"2\" : \n{\n\"CODE\" : \"Mixture\",\n\"DESC\" : \"Workcenter2\"\n},\n\"3\" :\n{\"CODE\" : \"Mixture\",\n\"DESC\" : \"Workcenter3\"\n}\n}\n}";
#include <ArduinoJson.h>
#include <String.h>
void setup() 
{
  Serial.begin(9600);
  StaticJsonBuffer<400> jsonBuffer; 
  Serial.println(json);
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()){
    Serial.println("parseObject() failed");
    return;
    }

  const char* ruidId = root["RUID"]["ID"]; 
  const char* macId = root["RUID"]["MACID"]; 
  const char* CompanyCode = root["COMPANY"]["CODE"]; 
  const char* CompanyDesc = root["COMPANY"]["DESC"]; 
  const char* plantCode = root["PLANT"]["CODE"];
  const char* plantDesc = root["PLANT"]["DESC"];
  const char* workCenter_01_Code = root["WORKCENTER"]["1"]["CODE"];
  const char* workCenter_01_Desc = root["WORKCENTER"]["1"]["DESC"];
  const char* workCenter_02_Code = root["WORKCENTER"]["2"]["CODE"];
  const char* workCenter_02_Desc = root["WORKCENTER"]["2"]["DESC"];
  const char* workCenter_03_Code = root["WORKCENTER"]["3"]["CODE"];
  const char* workCenter_03_Desc = root["WORKCENTER"]["3"]["DESC"];
  Serial.print("ruidId :");
  Serial.println(ruidId);
  Serial.print("macId :");
  Serial.println(macId);
  Serial.print("CompanyCode :");
  Serial.println(CompanyCode);
  Serial.print("CompanyDesc :");
  Serial.println(CompanyDesc);
  Serial.print("plantCode :");
  Serial.println(plantCode);
  Serial.print("plantDesc :");
  Serial.println(plantDesc);
  Serial.print("workCenter_01_Code :");
  Serial.println(workCenter_01_Code);
  Serial.print("workCenter_01_Desc :");
  Serial.println(workCenter_01_Desc);
  Serial.print("workCenter_02_Code :");
  Serial.println(workCenter_02_Code);
  Serial.print("workCenter_02_Desc :");
  Serial.println(workCenter_02_Desc);
  Serial.print("workCenter_03_Code :");
  Serial.println(workCenter_03_Code);
  Serial.print("workCenter_03_Desc :");
  Serial.println(workCenter_03_Desc);
  
}


void loop()
{
  
}

/*

"{ 
  /"RUID/":
  {
    /"ID/"  : /"RU_01/",
    /"ID/"  : /"RU_02/",
    /"MACID/" : /"00:00:00:00:00:00/"
  },
  /"COMPANY/":  
  { 
    /"CODE/"  : /"MUCR/",
    /"DESC/"  : /"Murdeshwar Ceramics/"
  },
  /"PLANT/" :
  {
    /"CODE/"  : /"MUP1/",
    /"DESC/"  : /"Plant1/"
  },
  /"WORKCENTER/":
  {
    /"1/":
    {
      /"CODE/"  : /"Mixture/",
      /"DESC/"  : /"Workcenter1/"
    },
    /"2/":
    {
      /"CODE/"  : /"Mixture/",
      /"DESC/"  : /"Workcenter2/"
    },
    /"3/":
    {
      /"CODE/"  : /"Mixture/",
      /"DESC/"  : /"Workcenter3/"
    }
  }
}

*/
