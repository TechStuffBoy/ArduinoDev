// Values For millis() in Dot Function
unsigned long previousMillis = 0; //for dot
const long interval = 1000; //for dot
char getPayloadLength;
void setup(){
  // setup run only once
  Serial.begin(9600);
  dot();
}

void loop(){
  
}
void dot ()
{
      int i=0;
      while(i<4){
          unsigned long currentMillis = millis();
          while(currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          i++;
          if( Serial.available()) {
                getPayloadLength = Serial.read();
          if( getPayloadLength == 'a') {
                Serial.println("Ended ");
                return;
          }
          }
         
          }
      if(i == 0){
          Serial.println(" One Dot ");
      }
      else if(i==1){
          Serial.println(" Two Dot ");
      }
      else if(i==2){
          Serial.println(" Three Dot ");
      } 
      else if(i == 3){
          Serial.println("     ");
      }
      else if(i>=4){
          i=0;
      }
      
      }
}      


