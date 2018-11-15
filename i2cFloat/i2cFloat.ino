#include<Wire.h>
float tempFloat = 35.48;
String val = "";
unsigned int tempInt = 0;

char t[7];
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    tempInt = tempFloat*100;
    val += tempFloat;
    sendData(9,1);
    sendData(99,2);
    sendData(999,3);
    sendData(9999,4);
    sendData(999.999,5);
    while(1);
}



void sendData(float temp,int id)
{
   unsigned long tempInt = 0;
   String valStr = "";
   tempInt = temp*1000;
   if(tempInt <= 9999){
     valStr = "";
     valStr += id;
     valStr += "000"; 
     valStr += tempInt;
   } else if( tempInt <= 99999 ){
     valStr = "";
     valStr += id;
     valStr += "00";
     valStr += tempInt;
   } else if( tempInt <= 999999 ){
     valStr = "";
     valStr += id;
     valStr += "0";
     valStr += tempInt;
   } else if ( tempInt <= 9999999){
      valStr = "";
      valStr += id;
      //valStr += "0";
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


//void sendData(float temp)
//{
//   unsigned long tempInt = 0;
//   String valStr = "";
//   tempInt = temp*1000;
//   if(tempInt <= 9999){
//     valStr = "";
//     valStr += "0000"; 
//     valStr += tempInt;
//   } else if( tempInt <= 99999 ){
//     valStr = "";
//     valStr += "000";
//     valStr += tempInt;
//   } else if( tempInt <= 999999 ){
//     valStr = "";
//     valStr += "00";
//     valStr += tempInt;
//   } else if ( tempInt <= 9999999){
//      valStr = "";
//      valStr += "0";
//      valStr += tempInt;
//   } else if ( tempInt <= 99999999){
//        valStr = "";
//        valStr += tempInt;
//   }
//   
//   int len = valStr.length();
//   byte byteT[len+1];
//   valStr.getBytes(byteT,len+1);
//   for(int i=0;i<len+1;i++){
//   Serial.print(byteT[i]);//we will recieve only the ascii value;
//   Serial.print("\t");
//   }
//   Serial.println();
//   Wire.write(byteT,len+1);
//
//}




//void sendData(float temp)
//{
//   unsigned long tempInt = 0;
//   String valStr = "";
//   tempInt = temp*1000;
//   //Serial.print("TempInt:");
//   //Serial.println(tempInt);
//   if(tempInt <= 9999){
//     valStr = "";
//     valStr += "0000"; 
//     valStr += tempInt;
//   } else if( tempInt <= 99999 ){
//     valStr = "";
//     valStr += "000";
//     valStr += tempInt;
//   } else if( tempInt <= 999999 ){
//     valStr = "";
//     valStr += "00";
//     valStr += tempInt;
//   } else if ( tempInt <= 9999999){
//      valStr = "";
//      valStr += "0";
//      valStr += tempInt;
//   } else if ( tempInt <= 99999999){
//        valStr = "";
//        valStr += tempInt;
//   }
//   
//   int len = valStr.length();
//   Serial.print("len:");
//   Serial.println(len);
//   byte byteT[len+1];
//   valStr.getBytes(byteT,len+1);
//   for(int i=0;i<len+1;i++){
//   Serial.print(byteT[i]);//we will recieve only the ascii value;
//   Serial.print("\t");
//   }
//   Serial.println();
//   Wire.write(byteT,len+1);
//
//}


//void sendData(float temp)
//{
//   unsigned long tempInt = 0;
//   String valStr = "";
//   tempInt = temp*1000;
//   //Serial.print(tempInt);
//   valStr += tempInt;
//   int len = valStr.length();
//   byte byteT[len+1];
//   valStr.getBytes(byteT,len+1);
//   for(int i=0;i<len+1;i++){
//   Serial.print(byteT[i]);//we will recieve only the ascii value;
//   Serial.print("\t");
//   }
//   Serial.println();
//   Wire.write(byteT,len+1);
//}

//void sendData(float temp)
//{
//   String valStr = "";
//   valStr += tempFloat;
//   int len = valStr.length();
//   char t[len];
//   byte byteT[len];
//   dtostrf(temp,len,3,t); 
//   Serial.println(t);
//   for(int i=0;i<len;i++){
//    byteT[i]=t[i];
//   }
////   for(int i=0;i<len;i++){
////    Serial.print(byteT[i]);
////   }
////   Serial.println();
//   //Wire.write(byteT,len);
//}

