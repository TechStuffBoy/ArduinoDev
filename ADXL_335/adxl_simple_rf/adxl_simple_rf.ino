#include<Wire.h>
#define SLAVE_ADDRESS 0x04
byte antennaPin = 3;

byte antennaValCur = 0;
int valXCur = 0;
int valYCur = 0;
int valZCur = 0;

byte xPin = 0;
byte yPin = 1;
byte zPin = 2;
byte data[8];
int i = 0; // need for monitoring location of tool
int antenaData = 0;
int ZgramCur = 0;
int XgramCur = 0;
int YgramCur = 0;
void setup() {
    analogReference(EXTERNAL);
    Serial.begin(9600);
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendAllData);
    pinMode(antennaPin, INPUT);
}

void loop() {
    
      while(i<20){
        delay(100);
        antennaValCur = digitalRead(antennaPin);
        if(antennaValCur){
            break;
        } else
            i++;
      }
      if(i<20){
           antenaData = 1;
       } else {
           antenaData = 0;
       }
       i=0;
      Serial.print("antenaData :");
      Serial.println(antenaData);
}

void sendAllData()
{
    valXCur = analogRead(xPin);
    valYCur = analogRead(yPin);
    valZCur = analogRead(zPin);
    antennaValCur = digitalRead(antennaPin);
    XgramCur = (valXCur - 527.0)*0.330;
    YgramCur = (valYCur - 527.0)*0.330;
    ZgramCur = (valZCur - 527.0)*0.330;
//    Serial.print("XgramCur : ");
//    Serial.println(XgramCur);
//    Serial.print("YgramCur : ");
//    Serial.println(YgramCur);
//    Serial.print("ZgramCur : ");
//    Serial.println(ZgramCur);
    data[0] = XgramCur&0x7F;
    data[1] = (XgramCur & 0xFF80) >> 7;
    data[2] = YgramCur&0x7F;
    data[3] = (YgramCur & 0xFF80) >> 7;
    data[4] = ZgramCur&0x7F;
    data[5] = (ZgramCur & 0xFF80) >> 7;
    data[6] = antenaData;
    data[7] = 0;
    Wire.write(data, 8);
}

