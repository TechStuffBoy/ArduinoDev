
 unsigned int in=0;
unsigned int entering=0;
unsigned int leaving=0;
unsigned int total=0;
unsigned int out=0;
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
 if((val_1==HIGH && val_2==LOW))
    in=1;
 if((val_2==HIGH && val_1==LOW))
    out=1;

    if(in > out)
        entering=1;
     if(out>in)
        leaving=1;
   actual_strength();     
}
 void actual_strength()
  {
        
      
        if((in+out)==2)
          {
            if(entering==1)
              total=total+1;
            else if(leaving==1)
              total=total-1; 
          }
        Serial.println("Actual Strength in Room :");
        Serial.println(total);
        entering=0;
        leaving=0;
        in=0;
        out=0;
    }

