byte fwd1 =2;
byte fwd2=3;
byte rev1=4;
byte rev2=5;

void setup(){
  
}

void loop(){
  forward();
  
  //backward();
}

void forward(){
  digitalWrite(fwd1,HIGH);
  delay(10);
//  digitalWrite(fwd1,LOW);
//  delay(2);
  digitalWrite(fwd2,HIGH);
  delay(10);
//  digitalWrite(fwd2,LOW);
//  delay(2);
}
void backward(){
  digitalWrite(rev1,LOW);
  delay(2);
  digitalWrite(rev2,LOW);
  delay(2);
}

