#include<Wire.h>
long tempInt = 0;
String valStr = "";
int len = 0;
byte sign[6]; 
byte byteT[10];
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {

    /*
XgramCur : -22.44   YgramCur : -10.89   ZgramCur : 23.43
XgramPrev : -22.11  YgramPrev : -9.90   ZgramPrev : 23.76
dxdtCur : -0.44 dydtCur : -1.32 dzdtCur : -0.44
dxdtPrev : 0.44 dydtPrev : 0.00 dzdtPrev : 0.00
<-----------------------END---------------------->
<-----------------------START---------------------->
XgramCur : -22.44   YgramCur : -11.22   ZgramCur : 23.10
XgramPrev : -22.44  YgramPrev : -10.89  ZgramPrev : 23.43
dxdtCur : 0.00  dydtCur : -0.44 dzdtCur : -0.44
dxdtPrev : -0.44    dydtPrev : -1.32    dzdtPrev : -0.44

     */
    sendData(-22.44,1);
    dataParser(byteT,10);
    sendData(-10.89,2);
    dataParser(byteT,10);
    sendData(23.10,3);
    dataParser(byteT,10);
    while(1);
}

void sendData(float temp,int id)
{
   tempInt = 0;
   valStr = "";
   tempInt = temp*1000;
   Serial.println(tempInt);
   if(tempInt < 0) {
        tempInt = -tempInt;
        sign[id] = 1;
   } else {
        sign[id] = 0;
   }
   if(tempInt <= 9999){
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
    Serial.println(rawData);
    
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


//void sendData(float temp,int id)
//{
//   tempInt = 0;
//   valStr = "";
//   tempInt = temp*1000;
//   if(tempInt <= 9999){
//     valStr = "";
//     valStr += id;
//     valStr += "000"; 
//     valStr += tempInt;
//   } else if( tempInt <= 99999 ){
//     valStr = "";
//     valStr += id;
//     valStr += "00";
//     valStr += tempInt;
//   } else if( tempInt <= 999999 ){
//     valStr = "";
//     valStr += id;
//     valStr += "0";
//     valStr += tempInt;
//   } else if ( tempInt <= 9999999){
//      valStr = "";
//      valStr += id;
//      //valStr += "0";
//      valStr += tempInt;
//   } 
//   len = valStr.length();
//   valStr.getBytes(byteT,len+1);
//   for(int i=0;i<len+1;i++){
//       Serial.print(byteT[i]);//we will recieve only the ascii value;
//       Serial.print("\t");
//   }
//   Serial.println();
//   Wire.write(byteT,len+1);
//}

//void dataParser(byte *byteTemp,byte size)
//{
//    String rawData = "";
//    String numberStr ="";
//    String values[6];
//    long longNo[6];
//    float realData[6];
//    for(int i=0;i<(size-1);i++){
//        rawData += (byteTemp[i] - '0');
//    }
//    Serial.println(rawData);
//    for(byte i=1;i<=6;i++){
//        if(rawData.charAt(0) == (i+48)){
//            values[i] = "";
//            longNo[i] = 0;
//            realData[i] = 0;
//            values[i] += rawData.substring(1,8);
//            longNo[i] = values[i].toInt();
//            realData[i] = longNo[i]/1000.0;
//            Serial.print("realData[");
//            Serial.print(i);
//            Serial.print("] : ");
//            Serial.println(realData[i],3);
//        }
//    }
//}


//void dataParser(byte *byteTemp,byte size)
//{
//    String dataX = "";
//    String numberStr = "";
//    long longNo = 0;
//    float realData = 0;
//    for(int i=0;i<(size-1);i++){
//        dataX += (byteTemp[i] - '0');
//    }
//    Serial.println(dataX);
//    if(dataX.charAt(0) == '3'){
//        numberStr = "";
//        numberStr += dataX.substring(1,8);
//    }
//    Serial.print("numberStr :");
//    Serial.println(numberStr);
//    longNo = numberStr.toInt();
//    realData = longNo/1000.0;
//    Serial.print("realData :");
//    Serial.println(realData,3);
//}

