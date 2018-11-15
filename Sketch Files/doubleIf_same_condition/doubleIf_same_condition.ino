
int a = 5;

void setup(){
  Serial.begin(9600);
}
void loop(){

  if( a == 5){
    Serial.println("A is 5");
  }
   if( a == 5){
    Serial.println("A is not  5");
  }
}

