int sen1=2;
int sen2=3;
int val1=0;
int val2=0;


void setup()
{
  Serial.begin(9600);
  pinMode(sen1,INPUT);
  pinMode(sen2, INPUT);
}

void loop()
{
  val1=digitalRead(sen1);
  val2=digitalRead(sen2);
  if(val1>0 && val2==0)
  {
    Serial.print("in");
    
  }
  if(val1==0 && val2>0)
  {
    Serial.println("out");
  }
  delay(100);
}

  

