byte DI_00 = 37;
byte DI_01 = 36;
byte DI_02 = 35;
byte DI_03 = 34;

byte DO_00 = 33;
byte DO_01 = 32;
byte DO_02 = 31;
byte DO_03 = 30;

//PH0-->17,PH1-->16,PH2-->None,PH3-->6,PH4-->7,PH5-->8,PH6-->9,PH7-->T4
//PA0-->22,PA1-->23,PA2-->24,PA3-->25,PA4-->26,PA5-->27,PA6-->28,PA7-->29.
struct portStruct {
    byte bitMask;
    byte pin;
    boolean curState;
    boolean prevState;
};

portStruct diPorts[4] = {
                            {0b00000001, DI_00, false, false},
                            {0b00000010, DI_01, false, false},
                            {0b00000100, DI_02, false, false},
                            {0b00001000, DI_03, false, false}
                       };
                        /*
DDRC = 0b11110000; //30,31,32,33, 34,35,36,37 -->for mega
DDRA = 0b11111111 //29,28,27,26,25,24,23,22 --> for mega
bitmask has to be for specific ports and microcontroller.
*/
portStruct doPorts[4] = {
                            {0b00010000, DO_00, false, false},
                            {0b00100000, DO_01, false, false},
                            {0b01000000, DO_02, false, false},
                            {0b10000000, DO_03, false, false}
                        };

void setup()
{
    Serial.begin(9600);
    DDRC = 0b00001111; //30,31,32,33, 34,35,36,37
    //DDRA = 0b00001111; //29,28,27,26,25,24,23,22
    initPorts();
}

void loop()
{
    /*
    if(readDIPort(DI_00)) {
       Serial.println("Proximity One is ON");
       writeDOPort(DO_02,true);
       delay(250);
       writeDOPort(DO_02,false);
       delay(250);
    }

    if(readDIPort(DI_01)) {
       Serial.println("Proximity Two is ON");
       writeDOPort(DO_03,true);
       delay(250);
       writeDOPort(DO_03,false);
       delay(250);
    }
    */

    int forward = readDIPort(DI_00);
    int reverse = readDIPort(DI_01);

    if( (!forward && !reverse) || (forward && reverse) ){
        Serial.println("Not allowed");
        writeDOPort(DO_00,false);
        writeDOPort(DO_01,false);
    } else if( forward && !reverse ){
        Serial.println("Forward");
        writeDOPort(DO_00,true);
        writeDOPort(DO_01,false);
    } else if( !forward && reverse ){
        Serial.println("Reverse");
        writeDOPort(DO_00,false);
        writeDOPort(DO_01,true);
    }
    delay(100);
}


void writeDOPort(byte port,boolean value) 
{
    for(unsigned int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
        if( doPorts[i].pin == port){
            doPorts[i].prevState = doPorts[i].curState;
            doPorts[i].curState = value;
            if(doPorts[i].prevState != doPorts[i].curState){
                digitalWrite(port,value);
                //Serial.println("Value changing");
            }
            //else
              //  Serial.println("value not changing");
        }
    }
}

void initPorts()
{
   for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        pinMode(diPorts[i].pin ,INPUT);
  }

  for( unsigned int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
     pinMode(doPorts[i].pin ,OUTPUT);
  }
}

boolean readDIPort(byte inputPin)
{
    boolean val;
    byte recd = PINC; //change where DI is connected
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        if( diPorts[i].pin == inputPin ){
            val = (((recd & diPorts[i].bitMask) == 0)? false:true);
            return val;
        }
   }
}
