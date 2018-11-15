#include<SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

int incomingByte = 0;   // for incoming serial data
String data="";
int i=0;
byte incoming = 0;
void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
         mySerial.begin(9600);
}
void loop() {
    //46 to 57 ascii
    if (mySerial.available()) {
        incoming = mySerial.read();
        if(incoming == 13 ) {
            //Serial.println();
            Serial.print("Data :");
            Serial.println(data);
            data = "";
        } else {
            //Serial.print((char)incoming);
            data += (char)incoming;
        }
    } 
}

