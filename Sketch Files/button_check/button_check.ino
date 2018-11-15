int button=3;
int state=0;
int led=13;
int i=0;
void setup()
{
  pinMode(button,INPUT);
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  state=digitalRead(button);
  while(state){
   i++;
   break;
   }
  // Serial.println(i);
   if((i%2)){
    digitalWrite(led, HIGH);
    delay(50);
   }
     if(!(i%2)){
       digitalWrite(led, LOW);
       delay(50);
     }
}

