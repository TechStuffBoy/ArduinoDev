byte mac[] = {0x00, 0x11, 0x11, 0x00, 0x00, 0x01};
 String myMacId;

String getMacId()
{
    myMacId = "";
    for(int i = 0;i< sizeof(mac) ;i++ ){
        if( mac[i] < 9 ){
            myMacId += '0';
            myMacId += String(mac[i],HEX);
        } else {
            myMacId += String(mac[i],HEX); // ( mac[i]);
        }
        if( i < sizeof(mac) - 1)
            myMacId +="-";
        }
    return myMacId;
}
void setup()
{
    Serial.begin(9600);
    getMacId();
    //myMacId= String(mac);
    //Serial.println(myMacId);
    //Serial.println(mac[0]);
    //Serial.println(String(mac[1],HEX));
//    Serial.println(mac[2]);
//    Serial.println(mac[3]);
//    Serial.println(mac[4]);
//    Serial.println(mac[5]);
    Serial.println(myMacId);
    Serial.println();
}
void loop()
{
    
}

