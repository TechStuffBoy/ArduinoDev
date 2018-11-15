int in1=3;
int in2=4;
void setup()
{
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  Serial.begin(9600);
  
}
void loop()
{
  initial();
  if(Serial.available()>0){
    if(Serial.read()=='a'){
  trigger();
  delay(50);
  initial();
}
}
}
void initial()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  }
void trigger()
{
  Serial.println("Im in Trigger");
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  
  }
