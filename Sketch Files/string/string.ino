String jsonSTRING1= "{\n\"Sensors\":{\n\"RDI000\":\"HIGH\"\n}\n}";
String jsonSTRING2= "{\n\"Sensors\":{\n\"RDI001\":\"LOW\"\n}\n}";
String jsonSTRING3="{\n\"order\":{\n\"RDO000\":\"HIGH\",\n\"RDO001\":\"HIGH\"\n}\n}";
String jsonSTRING4="{\n\"order\":{\n\"RDO001\":\"LOW\",\n\"RDO001\":\"LOW\"\n}\n}";

void setup(){
  Serial.begin(9600);
}
void loop(){
  Serial.println(jsonSTRING1);
  Serial.println(jsonSTRING2);
  Serial.println(jsonSTRING3);
  Serial.println(jsonSTRING4);
  Serial.println();
  delay(3000);
}

