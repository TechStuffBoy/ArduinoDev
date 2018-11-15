#include<Wire.h>
unsigned int valXCur = 0;
unsigned int valYCur = 0;
unsigned int valZCur = 0;

unsigned int valXPrev = 0;
unsigned int valYPrev = 0;
unsigned int valZPrev = 0;

int xMin = 529;
int xMAx = 526;

int yMin = 532;
int yMax = 532;

int zMin = 425;
int zMax = 630;

float XgramCur = 0.0;
float YgramCur = 0.0;
float ZgramCur = 0.0;

float XgramPrev = 0.0;
float YgramPrev = 0.0;
float ZgramPrev = 0.0;

float sampleInterval = 0.250;
float dxdtCur = 0.0;
float dydtCur = 0.0;
float dzdtCur = 0.0;

float dxdtPrev = 0.0;
float dydtPrev = 0.0;
float dzdtPrev = 0.0;

byte xPin = 0;
byte yPin = 1;
byte zPin = 2;

void setup() {
    analogReference(EXTERNAL);
    Serial.begin(9600);
    valXPrev = analogRead(xPin);
    valYPrev = analogRead(yPin);
    valZPrev = analogRead(zPin);
}

void loop() {

    valXPrev = valXCur;
    valYPrev = valYCur;
    valZPrev = valZCur;

    XgramPrev = XgramCur;
    YgramPrev = YgramCur;
    ZgramPrev = ZgramCur;

    dxdtPrev = dxdtCur;
    dydtPrev = dydtCur;
    dzdtPrev = dzdtCur;
    
     for(int i=0 ; i<50; i++){
       valXCur += analogRead(xPin);
       valYCur += analogRead(yPin);
       valZCur += analogRead(zPin);
       delay(5); 
    }
    valXCur = valXCur/50;
    XgramCur = (valXCur - 527.0)*0.330;

    valYCur = valYCur/50;
    YgramCur = (valYCur - 527.0)*0.330;
    
    valZCur = valZCur/50;
    //ZgramCur = (valZCur - 527.0)*0.0098;
    ZgramCur = (valZCur - 527.0)*0.330;

    dxdtCur = ( XgramCur - XgramPrev) / (1.0 - sampleInterval);
    dydtCur = ( YgramCur - YgramPrev) / (1.0 - sampleInterval);
    dzdtCur = ( ZgramCur - ZgramPrev) / (1.0 - sampleInterval);
    //findStroke(dxdtCur,dxdtPrev);

    Serial.println("<-----------------------START---------------------->");

    Serial.print("XgramCur : ");
    Serial.print(XgramCur);
    Serial.print("\tYgramCur : ");
    Serial.print(YgramCur);
    Serial.print("\tZgramCur : ");
    Serial.println(ZgramCur);
    
    Serial.print("XgramPrev : ");
    Serial.print(XgramPrev);
    Serial.print("\tYgramPrev : ");
    Serial.print(YgramPrev);
    Serial.print("\tZgramPrev : ");
    Serial.println(ZgramPrev);
    
    Serial.print("dxdtCur : ");
    Serial.print(dxdtCur);
    Serial.print("\tdydtCur : ");
    Serial.print(dydtCur);
    Serial.print("\tdzdtCur : ");
    Serial.println(dzdtCur);
   

    Serial.print("dxdtPrev : ");
    Serial.print(dxdtPrev);
    Serial.print("\tdydtPrev : ");
    Serial.print(dydtPrev);
    Serial.print("\tdzdtPrev : ");
    Serial.println(dzdtPrev);

    Serial.println("<-----------------------END---------------------->");
    delay(100);
}

void findStroke( float dmdtCur, float dmdtPrev) 
{
    
//    if(dmdtCur<0){
//        dmdtCur = -dmdtCur;
//    }
//    if(dmdtPrev<0){
//        dmdtPrev = -dmdtPrev;
//    }
    Serial.println(dmdtCur - dmdtPrev);
}

void sendData(float temp)
{
   unsigned long tempInt = 0;
   String valStr = "";
   tempInt = temp*1000;
   if(tempInt <= 9999){
     valStr = "";
     valStr += "0000"; 
     valStr += tempInt;
   } else if( tempInt <= 99999 ){
     valStr = "";
     valStr += "000";
     valStr += tempInt;
   } else if( tempInt <= 999999 ){
     valStr = "";
     valStr += "00";
     valStr += tempInt;
   } else if ( tempInt <= 9999999){
      valStr = "";
      valStr += "0";
      valStr += tempInt;
   } else if ( tempInt <= 99999999){
        valStr = "";
        valStr += tempInt;
   }
   
   int len = valStr.length();
   byte byteT[len+1];
   valStr.getBytes(byteT,len+1);
   for(int i=0;i<len+1;i++){
   Serial.print(byteT[i]);//we will recieve only the ascii value;
   Serial.print("\t");
   }
   Serial.println();
   Wire.write(byteT,len+1);

}
