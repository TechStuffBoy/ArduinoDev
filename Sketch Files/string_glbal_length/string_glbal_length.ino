#include<String.h>
//String initializations
String myMacId = "11-11-11-00-00-01";
String clientId = "RU_" + myMacId;
String clientState = "client/state/RU_" + myMacId;
String getRuDetail = "GET_RU_DETAIL";
String setRuDetail = "SET_RU_DETAIL_" + myMacId;

//char Buffers
char clientIdBuff[50];
char clientStateBuff[50];
char getRuDetailBuff[50];
char setRuDetailBuff[50];



void setup()
{
//  //Strings to char array
clientId.toCharArray(clientIdBuff,(clientId.length()+1));
clientState.toCharArray(clientStateBuff,(clientState.length()+1));
getRuDetail.toCharArray(getRuDetailBuff,(getRuDetail.length()+1));
setRuDetail.toCharArray(setRuDetailBuff,(setRuDetail.length()+1));
  Serial.begin(9600);
  Serial.println(clientIdBuff);
  Serial.println(clientStateBuff);
  Serial.println(getRuDetailBuff);
  Serial.println(setRuDetailBuff);
  Serial.println(sizeof(setRuDetailBuff));
}

void loop(){
  
 }

