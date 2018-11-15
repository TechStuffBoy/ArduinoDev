String errorJson;
int errorJsonLen;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

errorJsonLen =errorJson.length();
if(errorJsonLen) {
 Serial.println(" i am pass ");
}
errorJson="hi";
errorJsonLen =errorJson.length();
if(errorJsonLen) {
 Serial.println(" i am hi ");
}
errorJson="";
errorJsonLen =errorJson.length();
if(errorJsonLen) {
 Serial.println(" i am \"\"");
}
}
void loop() {
  // put your main code here, to run repeatedly: 
}
