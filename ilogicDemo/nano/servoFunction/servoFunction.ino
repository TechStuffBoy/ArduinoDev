/*
 * <-- Connection For a Servo in Arduino Nano -->
 * Servo Forward --> Digital Pin 2 [Input]
 * Servo Reverse --> Digital Pin 3 [Input]
 * Servo Signal --> A3(Analog Pin 3) [configured as Digital 
 * O/p internally using servo library]
 * 
 * <-- Connection For a Stepper in Arduino Nano-->
 * Stepper Forward --> Digital Pin 4[Input]
 * Stepper Reverse --> Digital Pin 5[Input]
 * Coil_1_Input_1 --> Digital Pin 7[Output]
 * Coil_1_Input_2 --> Digital Pin 8[Output]
 * Coil_2_Input_1 --> Digital Pin 9[Output]
 * Coil_2_Input_2 --> Digital Pin 10[Output]
 * 
 * 
 * D0(RX)-->PD0,D1(TX)-->PD1,D2-->PD2,D3-->PD3,D4-->PD4,D5-->PD5,D6-->PD6,D7-->PD7
 * D8-->PB0,D9-->PB1,D10-->PB2,D11-->PB3,D12-->PB4,D13-->PB5.(PB6 and PB7 is mapped to A6,A7.
 */

#include<Servo.h>
Servo trolly;
/*For Digital Input apprpriate PINx register should be set in readDIPort Method */
byte DI_00 = 2; //PD2,ServoFwd
byte DI_01 = 3; //PD3,ServoRev
byte DI_02 = 4; //PD4,StepperFwd
byte DI_03 = 5; //PD5,StepperRev

//Servo Signal is connected to A3

byte DO_00 = 7; //PD7
byte DO_01 = 8; //PB0
byte DO_02 = 9; //PB1
byte DO_03 = 10; //PB2

struct portStruct {
    byte bitMask;
    byte pin;
    boolean curState;
    boolean prevState;
};
//7,6,5,4,3,2,1,0
//A7,A6,13,12,11,10,9,8
portStruct diPorts[4] = {
                            {0b00000100, DI_00, false, false},
                            {0b00001000, DI_01, false, false},
                            {0b00010000, DI_02, false, false},
                            {0b00100000, DI_03, false, false}
};

portStruct doPorts[4] = {
                            {0b10000000, DO_00, false, false},
                            {0b00000001, DO_01, false, false},
                            {0b00000010, DO_02, false, false},
                            {0b00000100, DO_03, false, false}
                        };
void setup()
{
    Serial.begin(9600);
    DDRD = 0b11000010; //7,6,5,4,3,2,1,0
    DDRB = DDRB | 0b00000111; //A7,A6,13,12,11,10,9,8
    //This operation will not touch the pins we dont want and those pins will remain in theri old state.
    initServo();
    initPorts();
    
}

void loop()
{      
    rotateServo(DI_00,DI_01);//DI_00->Fwd,DI_01->Reverse
    delay(30);
}

void initServo()
{
    trolly.attach(A3); // servo attached to Analog Pin 3
    trolly.write(90); // Initially the servo must be stopped 
}
void rotateServo(byte FWD,byte REV)
{
    int forward = readDIPort(FWD);
    int reverse = readDIPort(REV);

    if( (!forward && !reverse) || (forward && reverse) ){
        trolly.write(90); //Stop the Servo
    } else if( forward && !reverse ){
        trolly.write(0); //Rotate Clockwise or Forward
    } else if( !forward && reverse ){
        trolly.write(180);
    }
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


boolean readDIPort(byte inputPin)
{
    boolean val;
    byte recd = PIND;
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        if( diPorts[i].pin == inputPin ){
            val = (((recd & diPorts[i].bitMask) == 0)? false:true);
            return val;
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
