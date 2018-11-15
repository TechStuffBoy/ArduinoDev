int sen=3;
int val=0;
int count=0;
bool check=true;
void setup(){
  pinMode(sen,INPUT);
  Serial.begin(9600);
}
void loop(){
  val=digitalRead(sen);
  while(val != 0)
  {
    val=digitalRead(sen);
    while(val == 0)
    {
      count++;
      Serial.println(count);
      break;
    }
  }
}
