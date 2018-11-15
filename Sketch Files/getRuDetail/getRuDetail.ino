unsigned long currentMillisToAskRuDetail;
unsigned long previousMillisToAskRuDetail = 0;
unsigned int intervalToToAskRuDetail = 10000;
boolean askRuDetailFlag = false;
void setup()
{
  Serial.begin(9600);
  askRuDetailFlag = true;
}

void loop(){
  if ( askRuDetailFlag == true ){
    AskRuDetail(0);
  }
}

void AskRuDetail(int errorJsonLen) 
{
    currentMillisToAskRuDetail = millis();
    if(currentMillisToAskRuDetail - previousMillisToAskRuDetail >= intervalToToAskRuDetail) {
          previousMillisToAskRuDetail = currentMillisToAskRuDetail; // {\"MAC_ID\" :<11-11-11-00-00-01>}
         Serial.println(" client.publish(getRuDetailBuff, myMacIdBuff,true)");
         
          }
//    if(getPayloadLength > 0 && topicCallback == "RU_DETAIL_<11-11-11-00-00-01>") {
//          everyFiveSeconds = false;
//    }
//    if(getPayloadLength > 0 && topicCallback == "RU_DETAIL_<11-11-11-00-00-01>" && errorJsonLen ) {
//          everyFiveSeconds = true;
//          errorJsonLen = 0;
//          errorJson ="";
//      }
//    topicCallback=""; 
}
