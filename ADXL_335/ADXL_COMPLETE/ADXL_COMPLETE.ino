#include<Wire.h>
#define SLAVE_ADDRESS 0x04

byte antennaValCur = 0;
int valXCur = 0;
int valYCur = 0;
int valZCur = 0;

byte xPin = 0;
byte yPin = 1;
byte zPin = 2;
byte data[8];
int i = 0; // need for monitoring location of tool

int ZgramCur = 0;
int XgramCur = 0;
int YgramCur = 0;

byte antennaPin = 3;//AntenaPin
byte dataPin1 = 4; //PD4,ServoFwd
byte dataPin2 = 5; //PD5,ServoRev
int whichMachine = 0;

byte dataPin1Val = 0;
byte dataPin2Val = 0;

struct portStruct {
    byte bitMask;
    byte pin;
    volatile unsigned char *hwPort;
    boolean pullupFlag;
    volatile unsigned char *pullup;
};

portStruct diPorts[3] = {
                            {0b00001000, antennaPin,&PIND, false,&PORTD},
                            {0b00010000, dataPin1,&PIND, false,&PORTD},
                            {0b00100000, dataPin2,&PIND, false,&PORTD}
};

void setup() {
     DDRD |= 00000010; //7,6,5,4,3,2,1,0
    //Setting PD3, PD4 and PD5 as input.
    analogReference(EXTERNAL);
    Serial.begin(9600);
    initPorts();
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendAllData);
    pinMode(antennaPin, INPUT);
}

void loop() {

      dataPin1Val = digitalRead(dataPin1);
      dataPin2Val = digitalRead(dataPin2);
      while(i<20){
        delay(100);
        antennaValCur = digitalRead(antennaPin);
        if(antennaValCur){
            break;
        } else
            i++;
      }
      if(i<20){
           if(dataPin1Val){
                whichMachine = 1;
           } else if(dataPin2Val){
                whichMachine = 2;
           }
       } else {
           whichMachine = 0;
       }
       i=0;
      
}

void initPorts()
{
   for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        pinMode(diPorts[i].pin ,INPUT);
  }
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
    data[6] = whichMachine;
    data[7] = 0;
    Wire.write(data, 8);
}

boolean readDIPort(byte inputPin)
{
    boolean val;
    byte recd;
    //byte recd = PINC; //change where DI is connected
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        if(diPorts[i].pullupFlag){
            *diPorts[i].pullup |= diPorts[i].bitMask;
            recd = *diPorts[i].hwPort;
        } else
            recd = *diPorts[i].hwPort;
        if( diPorts[i].pin == inputPin ){
            val = (((recd & diPorts[i].bitMask) == 0)? false:true);
            return val;
        }
   }
}
