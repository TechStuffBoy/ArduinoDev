const char* hi;
String g;

void setup(){
  hi="hello";
  Serial.begin(9600);
}
void loop(){

  g=hi;
  Serial.println(g);
  
}

