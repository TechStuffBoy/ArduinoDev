unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 1000;

unsigned long currentMillis1;
unsigned long previousMillis1 = 0;
const long interval1 = 2000;

unsigned long currentMillis2;
unsigned long previousMillis2 = 0;
const long interval2 = 3000;

unsigned long currentMillis3;
unsigned long previousMillis3 = 0;
const long interval3 = 4000;

unsigned long currentMillis4;
unsigned long previousMillis4 = 0;
const long interval4 = 5000;

boolean flag1 = true;
boolean flag2 = true;
boolean flag3 = true;
boolean flag4 = true;
boolean flag5 = true;

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  currentMillis =millis();
//  currentMillis1 =millis();
//  currentMillis2 =millis();
//  currentMillis3 =millis();
//  currentMillis4 =millis();
   if( flag5 == true ){
  currentMillis4 =millis();
  while(currentMillis4 - previousMillis4>= interval4) {
      previousMillis4 = currentMillis4;
      Serial.println("I am in condition5");
      flag5 = false;
  }           
  if (flag1 == true){
  
  while(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.println("I am in condition1");
      flag1 = false;
     }
  }
  if( flag2 == true ) {
   currentMillis1 =millis();
  while(currentMillis1 - previousMillis1 >= interval1) {
      previousMillis1 = currentMillis1;
      Serial.println("I am in condition2");
      flag2 = false;
     }
  }
  if( flag3 == true ) {
  currentMillis2 =millis();
  while(currentMillis2 - previousMillis2 >= interval2) {
      previousMillis2 = currentMillis2;
      Serial.println("I am in condition3");
      flag3 = false;
     }
  }
  if( flag4 == true ){
   currentMillis3 =millis();
  while(currentMillis3 - previousMillis3 >= interval3) {
      previousMillis3 = currentMillis3;
      Serial.println("I am in condition4");
      flag4 = false;
     }  
  }  

 
}
}
