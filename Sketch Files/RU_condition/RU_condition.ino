 if( gatewayConnect == false){
     dot(120,130,1,Black);
  }
  
if( homManyTime == true){
  
    if( errorScreenFlag == true ){
       gatewayConnect = true;
       errorScreen();
    }
 
    unsigned long currentMills1 = millis();
    while( currentMills1 - previousMillis1 >= interval1 ){
            currentMills1 = previousMillis1;
            errorScreenFlag = false;
            if( fill == true){
                tft.fillScreen(White);
                fill = false;
            }
            Serial.println("i am in condition");
            StartScreen();
            gatewayConnect = false;
            homManyTime = false;
       }
}     
