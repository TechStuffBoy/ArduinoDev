#include<Wire.h>
#define SLAVE_ADDRESS 0x05

long tempInt = 0;
String valStr = "";
int len = 0;
byte sign[6];
byte byteT[10];

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
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendAllDatas);
    Wire.onReceive(recieveData);
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
  //  sendData(XgramCur,1);
//    dataParser(byteT,10);
//    delay(20);
//    sendData(YgramCur,2);
//    dataParser(byteT,10);
//     delay(20);
//    sendData(ZgramCur,3);
//    dataParser(byteT,10);
//     delay(20);
//    sendData(dxdtCur,4);
//    dataParser(byteT,10);
//     delay(20);
//    sendData(dydtCur,5);
//    dataParser(byteT,10);
//     delay(20);
   // sendData(dzdtCur,6);
    //dataParser(byteT,10);
    delay(20);
    /*
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
    */
    delay(100);
}

void sendData(float temp,int id)
{
   tempInt = 0;
   valStr = "";
   tempInt = temp*1000;
   //Serial.println(tempInt);
   if(tempInt < 0) {
        tempInt = -tempInt;
        sign[id] = 1;
   } else {
        sign[id] = 0;
   }
   Serial.println(tempInt);
     if(tempInt == 0){
        valStr = "";
        valStr += id;
        valStr += sign[id];
        valStr += "0000000"; 
     } else if(tempInt <= 999){
         valStr = "";
         valStr += id;
         valStr += sign[id];
         valStr += "0000"; 
         valStr += tempInt;
     }
     else if(tempInt <= 9999){
     valStr = "";
     valStr += id;
     valStr += sign[id];
     valStr += "000"; 
     valStr += tempInt;
   } else if( tempInt <= 99999 ){
     valStr = "";
     valStr += id;
     valStr += sign[id];
     valStr += "00";
     valStr += tempInt;
   } else if( tempInt <= 999999 ){
     valStr = "";
     valStr += id;
     valStr += sign[id];
     valStr += "0";
     valStr += tempInt;
   } else if ( tempInt <= 9999999){
      valStr = "";
      valStr += id;
      valStr += sign[id];
      //valStr += "0";
      valStr += tempInt;
   } 
   len = valStr.length();
   valStr.getBytes(byteT,len+1);
   for(int i=0;i<len+1;i++){
       Serial.print(byteT[i]);//we will recieve only the ascii value;
       Serial.print("\t");
   }
   Serial.println();
   Wire.write(byteT,len+1);
   
}
void sendAllDatas()
{
    sendData(XgramCur,1);
    sendData(YgramCur,2);
//    sendData(ZgramCur,3);
//    sendData(dxdtCur,4);
//    sendData(dydtCur,5);
//    sendData(dzdtCur,6);
}

void dataParser(byte *byteTemp,byte size)
{
    String rawData = "";
    String numberStr ="";
    String values[6];
    long longNo[6];
    float realData[6];
    for(int i=0;i<(size-1);i++){
        rawData += (byteTemp[i] - '0');
    }
    //Serial.println(rawData);
    for(byte i=1;i<=6;i++){
        if(rawData.charAt(0) == (i+48)){
            if(rawData.charAt(1) == '1'){
                sign[i+48] = 0;
                values[i] = "";
                longNo[i] = 0;
                realData[i] = 0;
                values[i] += rawData.substring(2,9);
                longNo[i] = values[i].toInt();
                realData[i] = -(longNo[i]/1000.0);
                Serial.print("realData[");
                Serial.print(i);
                Serial.print("] : ");
                Serial.println(realData[i],3);
            } else {
                values[i] = "";
                longNo[i] = 0;
                realData[i] = 0;
                values[i] += rawData.substring(2,9);
                longNo[i] = values[i].toInt();
                realData[i] = longNo[i]/1000.0;
                Serial.print("realData[");
                Serial.print(i);
                Serial.print("] : ");
                Serial.println(realData[i],3);
            }
        }
    }
    
}

