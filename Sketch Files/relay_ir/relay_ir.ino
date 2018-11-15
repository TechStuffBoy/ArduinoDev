int ir_sen=3;
int relay=4;
int val=0;
void setup(){
  Serial.begin(9600);
  pinMode(ir_sen,INPUT);
  pinMode(relay,OUTPUT);
  }
  void loop(){
    val=digitalRead(ir_sen);

    if(val){
      digitalWrite(relay,HIGH);
      Serial.println("Relay on");
  }
  if(!val)
    digitalWrite(relay,LOW);
  
  }

