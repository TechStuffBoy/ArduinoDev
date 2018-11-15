byte DI_00 = 37;
byte DI_01 = 36;
byte DI_02 = 35;
byte DI_03 = 34;
byte DI_04 = 49;

//PA0-->22,PA1-->23,PA2-->24,PA3-->25,PA4-->26,PA5-->27,PA6-->28,PA7-->29.
struct portStruct {
    byte bitMask;
    byte pin;
    boolean curState;
    boolean prevState;
};

portStruct diPorts[5] = {
                            {0b00000001, DI_00, false, false},
                            {0b00000010, DI_01, false, false},
                            {0b00000100, DI_02, false, false},
                            {0b00001000, DI_03, false, false},
                            {0b10000000, DI_04, false, false}
};
boolean btnSte = false;
void setup()
{
    Serial.begin(9600);
   
    DDRC = 0b00000000; //30,31,32,33, 34,35,36,37
    DDRA = 0b11011111; //29,28,27,26,25,24,23,22
    DDRL = 0b00000000; //49,48,47,46,45,44,43,42
    pinMode(DI_04, INPUT);
}

void loop()
{
    //Serial.println("Val in sen 1 " + (String)readDIPort(DI_01));
    //Serial.println("Val in sen 0 " + (String)readDIPort(DI_00));
    if(readDIPort(DI_01))
        Serial.println("Proximity_Two_Found");
    if(readDIPort(DI_00))
        Serial.println("Proximity_One_Found");
    delay(1000);
}

boolean readDIPort(byte inputPin)
{
    boolean val;
    byte recd = PINC;
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        if( diPorts[i].pin == inputPin ){
            val = (((recd & diPorts[i].bitMask) == 0)? false:true);
            return val;
        }
   }
}


/*
void readDIPort(byte inputPin)
{
    boolean val;
    byte recd = PINC;
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        if( diPorts[i].pin == inputPin ){
           // Serial.println(recd);
            val = (((recd & diPorts[i].bitMask) == 0)? false:true);
            Serial.println(val);
            /*
            diPorts[i].curState = val;
            if( val != diPorts[i].prevState ){
               Serial.println("Pressed");
               //diPorts[i].prevState = diPorts[i].curState;
            }
            else
                Serial.println("Released;");
            diPorts[i].prevState = diPorts[i].curState;
           
        }
       
    }
    
}




void readDIPorts()
{
     boolean val;
     byte recd = PINC;
     Serial.println(PINC,HEX);
     //Serial.println("recd = " + String(recd));
     for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        val = (((recd & diPorts[i].bitMask) == 0)? false:true);
        diPorts[i].curState = val;
        //Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
        
        if( val != diPorts[i].prevState ){
            if( val == true) {
            Serial.println("val true");
            }
            //publish(diPorts[i].portName, 
                //((val == true)? "true":"false"),false);
            //Serial.println(diPorts[i].portName+ ("---->  ")+String(val) + "     published");  
        }
        diPorts[i].prevState = diPorts[i].curState;
     }
}
*/
