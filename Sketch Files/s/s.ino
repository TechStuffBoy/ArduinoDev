unsigned long previousMillis2 = 0;
const long interval2 = 3000;
boolean errorFlag = false ;
boolean start= false;
String errorJson="";
String errorJson2="hello";
int errorJsonlen = 0;
int errorJsonlen2 = 0;
void setup(){
  Serial.begin(9600);
}

void loop(){
   errorJsonlen = errorJson.length();
   errorJsonlen2 = errorJson2.length();
  if(errorJsonlen){
    Serial.println(" Errorjson is valid although it has 0");
  }
  if(errorJsonlen2){
    Serial.println(" Errorjson2 is valid ");
  }
    if( errorFlag == false ) {
        unsigned long currentMillis2 = millis();
        while(currentMillis2 - previousMillis2 >= interval2) {
            previousMillis2 = currentMillis2;
            start = true;
            Serial.println("Error Loop");
            errorFlag =true;
            
     }
    if( start == true ){
          Serial.println("StartScreen()");
          Serial.println("gatewayConnect = false");
          Serial.println("everyFiveSeconds = true");
    }
  }
 } 
