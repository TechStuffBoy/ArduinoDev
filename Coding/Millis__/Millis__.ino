unsigned long interval=1000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.
 
bool ledState = false; // state variable for the LED
 
void setup() {
  Serial.begin(9600);
 pinMode(13, OUTPUT);
 digitalWrite(13, ledState);
}
 
void loop() {
 unsigned long currentMillis = millis();// grab current time
 Serial.println(" CurrentMillis() :");
 Serial.println( currentMillis);
 
 // check if "interval" time has passed (1000 milliseconds)
 if ((unsigned long)(currentMillis - previousMillis) >= interval) {
  
   ledState = !ledState; // "toggles" the state
   digitalWrite(13, ledState); // sets the LED based on ledState
   // save the "current" time
   previousMillis = currentMillis;
   
 }
 Serial.println("PreviousMillis() : ");
   Serial.println(previousMillis);
}
