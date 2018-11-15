byte DI_00 = 37;
byte DI_01 = 36;
byte DI_02 = 35;
byte DI_03 = 34;
byte DO_00 = 33;
byte DO_01 = 32;
byte DO_02 = 31;
byte DO_03 = 30;

struct portStruct {
    byte bitMask;
    String portName;
    byte pin;
    byte portNo;
    boolean curState;
    boolean prevState;
    
};
portStruct diPorts[4] = {
                            {0b00000001, "", DI_00, 0, false, false},
                            {0b00000010, "", DI_01, 1, false, false},
                            {0b00000100, "", DI_02, 2, false, false},
                            {0b00001000, "", DI_03, 3, false, false}
                        };

portStruct doPorts[4] = {
                            {0b00010000, "", DO_00, 0, false, false},
                            {0b00100000, "", DO_01, 1, false, false},
                            {0b01000000, "", DO_02, 2, false, false},
                            {0b10000000, "", DO_03, 3, false, false}
                        };

void setup()
{
    Serial.begin(9600);
    DDRB = 0b11110000; //30,31,32,33(outputs), 34,35,36,37(input)
    PORTB |= 0x06;
}
void loop()
{
    boolean on = false;
    boolean off = false;
    readDIPorts();

    if(Serial.available() > 0){
         int c = Serial.parseInt();
         Serial.println(c);
         
        /* if( c == 11 ){
            PORTB |= 0x02;
         }
         if( c == 10 ){
            PORTB & !( 0x02 );
         }
        */
    }
    
    /*
    for( int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++){
        on = PINC & !(doPorts[i].bitMask);
        off = PINC | doPorts[i].bitMask;

        if( Serial.available() ) {
            int c = Serial.parseInt();
            
        }
    
    }
    */
}

void readDIPorts()
{
     boolean val;
     byte recd = PINC;
     //Serial.println("recd = " + String(recd));
     for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        val = (((recd & diPorts[i].bitMask) == 0)? false:true);
        diPorts[i].curState = val;
        //Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
        
        if( val != diPorts[i].prevState ){
            if( val == true) {
               Serial.println("Counter of  " + String(i) );
            }
            Serial.println(diPorts[i].portName+ String(     val    ) + "published");  
        }
        diPorts[i].prevState = diPorts[i].curState;
     }
}

