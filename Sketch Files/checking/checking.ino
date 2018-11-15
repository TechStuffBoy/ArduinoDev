int sen1=4;
int sen2=5;
int val_1=0;
int val_2=0;
void setup()
{
 pinMode(sen1,INPUT);
 pinMode(sen2,INPUT);
  Serial.begin(9600);
}

void loop()
{
  val_1=digitalRead(sen1);
  val_2=digitalRead(sen2);
  if(val_1==HIGH)
  {
    Serial.println("hello from 1");
//    in_condition();
  }
  
  if(val_2==HIGH)
  
   {
    Serial.println("hello from 2");
  //  out_condition();
  }
}
