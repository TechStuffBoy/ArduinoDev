const int sen_1=2;
const int sen_2=3;
unsigned int val_1=0;
unsigned int val_2=0;
//unsigned int in=0;
//unsigned int out=0;
unsigned long previousMillis=0;
const long interval=1000;

void setup()
{
  pinMode(sen_1,INPUT);
  pinMode(sen_2,INPUT);
  Serial.begin(9600);
}
void loop()
{
  in_condition();
  out_condition();
}

void in_condition()
{
  if(digitalRead(sen_1) == HIGH  && digitalRead(sen_2)==LOW)
   in_check();
}  
void in_check()
{
 unsigned long currentMillis=millis();
 if(currentMillis- previousMillis>=interval )
  {
    previousMillis=currentMillis;

    if(digitalRead(sen_2) == HIGH  && digitalRead(sen_1)==LOW)
      Serial.println("Person in");
  }
 
}
void out_condition()
{
  if(digitalRead(sen_2)== HIGH  && digitalRead(sen_1)==LOW)
   out_check(); 
}
void out_check()
{
unsigned long currentMillis=millis();
  if(currentMillis- previousMillis>=interval)
  {
    previousMillis=currentMillis;

    if(digitalRead(sen_1) == HIGH  && digitalRead(sen_2)==LOW)
      Serial.println("Person out");
     
  }
   
}
  
