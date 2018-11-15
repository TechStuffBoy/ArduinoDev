byte DI_00 = 37;
byte DI_01 = 36;
byte DI_02 = 35;
byte DI_03 = 34;

byte DO_00 = 33;
byte DO_01 = 32;
byte DO_02 = 31;
byte DO_03 = 30;
byte DO_04 = 27;
byte DO_05 = 28;
byte DO_06 = 29;
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
// DDRC = 0b11110000; //30,31,32,33, 34,35,36,37 -->for mega
//DDRA = 0b11111111 //29,28,27,26,25,24,23,22 --> for mega
portStruct doPorts[7] = {
                            {0b00010000, DO_00, false, false},
                            {0b00100000, DO_01, false, false},
                            {0b01000000, DO_02, false, false},
                            {0b10000000, DO_03, false, false},
                            {0b00100000, DO_04, false, false},
                            {0b01000000, DO_05, false, false},
                            {0b10000000, DO_06, false, false}
                        };

void setup()
{
    Serial.begin(9600);
    DDRC = 0b11111111; //30,31,32,33, 34,35,36,37
    DDRA = 0b11111111; //29,28,27,26,25,24,23,22
}

void loop()
{
    writeDOPort(DO_00,true);
    delay(1000);
    writeDOPort(DO_00,false);
    delay(1000);

    writeDOPort(DO_01,true);
    delay(1000);
    writeDOPort(DO_01,false);
    delay(1000);

    writeDOPort(DO_04,true);
    delay(1000);
    writeDOPort(DO_04,false);
    delay(1000);

    writeDOPort(DO_05,true);
    delay(1000);
    writeDOPort(DO_05,false);
    delay(1000);
}


void writeDOPort(byte port,boolean value) 
{
    for(unsigned int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
        if( doPorts[i].pin == port){
            doPorts[i].prevState = doPorts[i].curState;
            doPorts[i].curState = value;
            if(doPorts[i].prevState != doPorts[i].curState){
                digitalWrite(port,value);
                Serial.println("Value changing");
            }else
                Serial.println("value not changing");
        }
    }
}


/*
void readDIPorts()
{
     boolean val;
     byte recd = PINC;
     //Serial.println("recd = " + String(recd));
     for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        val = (((recd & diPorts[i].bitMask) == 0)? false:true);
        diPorts[i].curState = val;
        //Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
        
        if( val != diPorts[i].prevState ){
            if( val == true) {
               counter[i] += 1; 
               Serial.println("Counter of  " + String(i) + "   " +  String(counter[i]));
            }
            publish(diPorts[i].portName, 
                ((val == true)? "true":"false"),false);
            Serial.println(diPorts[i].portName+ ("---->  ")+String(val) + "     published");  
        }
        diPorts[i].prevState = diPorts[i].curState;
     }
}
*/
