int i=0;
int t=0;
void setup(){
  Serial.begin(9600);
}
void loop(){
  while(i<45){
    i++;
    t++;
    delay(500);
    Serial.print("val:");
    Serial.print(t);
    
  }
  
}

