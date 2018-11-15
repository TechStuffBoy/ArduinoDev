boolean reconnectMqtt = false;
unsigned long currentMillisToReconnectMqtt;
unsigned long previousMillisToReconnectMqtt = 0;
unsigned int intervalToReconnectMqtt = 5000;
boolean assignCurrentTime = false;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  currentMillisToReconnectMqtt = millis();
  if(assignCurrentTime == true){
     while(currentMillisToReconnectMqtt - previousMillisToReconnectMqtt >= intervalToReconnectMqtt) {       
          previousMillisToReconnectMqtt = currentMillisToReconnectMqtt;
          Serial.println("I am in loop");
          assignCurrentTime = false;
          }
      
  }
  if(Serial.available()){
    char c = Serial.read();
    if ( c == 'a'){
      Serial.println("a");
      previousMillisToReconnectMqtt = currentMillisToReconnectMqtt;
      assignCurrentTime = true;
    }
  }
  }


