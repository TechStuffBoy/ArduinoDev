char a[]="hello";
#include<String.h>
void setup(){
  Serial.begin(9600);
}
void loop(){
   Serial.println(a);
   //memset ( a, 0, 20 );
    a ="kkkkkkkk";
   Serial.println(a);
   
   Serial.println("Hhhhhhhhhhhh");
   Serial.println(a);
}

