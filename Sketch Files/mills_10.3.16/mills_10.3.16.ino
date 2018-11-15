unsigned long previousMillis1 = 0;
const long interval1 = 5000;

unsigned long previousMillis = 0;
const long interval = 1000;

boolean flag = false;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  if( flag == false ){
  unsigned long currentMillis =millis();
  while(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.println("I am in condition");
      flag = true;
      break;
  }
  }
  unsigned long currentMillis1 =millis();
  while(currentMillis1 - previousMillis1 >= interval1) {
      previousMillis1 = currentMillis1;
      Serial.println("I passed 5 seconds");
     
  }
}
