unsigned long currentMillis;
unsigned long previousMillis = 0;
const int interval = 3000;

unsigned long currentMillis1;
unsigned long previousMillis1 = 0;
const int interval1 = 3000;

int timer1Count = 0;
int timer1CountCheck = 0;
int timer2Count = 0;
int timer2CountCheck = 0;
char c;
boolean timer1Flag = false;
boolean timer2Flag = false;
void setup(){
  Serial.begin(9600);
}

void loop(){
  currentMillis = millis();
  currentMillis1 = millis();
  if( timer1Flag == true){
    timer1();
    if ( timer1Count != timer1CountCheck ){
      timer1CountCheck = 0;
      timer1Count = 0;
      timer1Flag = false;
    }
  }
  if( timer2Flag == true){
    timer2();
     if ( timer2Count != timer2CountCheck ){
      timer2CountCheck = 0;
      timer2Count = 0;
      timer2Flag = false;
    }
  }
  if(Serial.available()){
          c = Serial.read();
          if( c == 'a'){
           previousMillis = currentMillis;
           timer1Flag = true;
           //timer2Flag = false;
           }
       
       if( c == 'b'){
           previousMillis1 = currentMillis1;
           //timer1Flag = false;
           timer2Flag = true;
           }
       }
}
void timer1(){
      if( currentMillis - previousMillis >= interval ){
            previousMillis = currentMillis;
            Serial.println("  I am in First Loop ");
            timer1CountCheck = timer1Count++;
      }
}

void timer2(){
      if( currentMillis1 - previousMillis1 >= interval1 ){
            previousMillis1 = currentMillis1;
            Serial.println("  I am in Second Loop ");
             timer2CountCheck = timer2Count++;
      }
}

