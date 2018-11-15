#include<Wire.h>
#define SLAVE_ADDRESS 0x04

int valXCur = 0;
int valYCur = 0;
int valZCur = 0;

byte xPin = 0;
byte yPin = 1;
byte zPin = 2;
byte data[6];
int ZgramCur = 0;
int XgramCur = 0;
int YgramCur = 0;

void setup() {
    analogReference(EXTERNAL);
    Serial.begin(9600);
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendAllData);
}

void loop() {
    valXCur = analogRead(xPin);
    valYCur = analogRead(yPin);
    valZCur = analogRead(zPin);
    XgramCur = (valXCur - 527.0)*0.330;
    YgramCur = (valYCur - 527.0)*0.330;
    ZgramCur = (valZCur - 527.0)*0.330;
//    Serial.print("XVal : ");
//    Serial.println(valXCur);
//    Serial.print("YVal : ");
//    Serial.println(valYCur);
//    Serial.print("ZVal : ");
//    Serial.println(valZCur);
    Serial.print("XgramCur : ");
    Serial.println(XgramCur);
    Serial.print("YgramCur : ");
    Serial.println(YgramCur);
    Serial.print("ZgramCur : ");
    Serial.println(ZgramCur);
    delay(10);
}

void sendAllData()
{
    data[0] = XgramCur&0x7F;
    data[1] = (XgramCur & 0xFF80) >> 7;
    data[2] = YgramCur&0x7F;
    data[3] = (YgramCur & 0xFF80) >> 7;
    data[4] = ZgramCur&0x7F;
    data[5] = (ZgramCur & 0xFF80) >> 7;
    Wire.write(data, 6);
}

