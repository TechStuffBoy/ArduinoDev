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
#include<Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
Servo trolly;
/*For Digital Input apprpriate PINx register should be set in readDIPort Method */
byte DI_00 = 2; //PD2,ServoFwd
byte DI_01 = 3; //PD3,ServoRev
byte DI_02 = 4; //PD4,StepperFwd
byte DI_03 = 5; //PD5,StepperRev
byte DI_04 =  11; // loadCell DOUT

//Servo Signal is connected to A3

byte DO_00 = 7; //PD7
byte DO_01 = 8; //PB0
byte DO_02 = 9; //PB1
byte DO_03 = 10; //PB2
byte DO_04 = 12; //loadCell SCK

byte servoFWD = 0 ;
byte servoREV = 0;
byte stepperFWD = 0;
byte stepperREV = 0;

byte valHigh = 127;
int timeMilli = 2;

byte potOnePin = 0;// Analog 0 port
byte potTwoPin = 1;
byte tempPin = 2;
byte vibPin = 6;

int potOneVal = 0;
int potTwoVal = 0;
int tempC = 0;
int vibrationValue = 0;
byte data[4]={0,0,0,0};

struct portStruct {
    byte bitMask;
    byte pin;
    volatile unsigned char *hwPort;
    boolean pullupFlag;
    volatile unsigned char *pullup;
};
//7,6,5,4,3,2,1,0
//A7,A6,13,12,11,10,9,8
portStruct diPorts[5] = {
                            {0b00000100, DI_00,&PIND, false,&PORTD},
                            {0b00001000, DI_01,&PIND, false,&PORTD},
                            {0b00010000, DI_02,&PIND, false,&PORTD},
                            {0b00100000, DI_03,&PIND, false,&PORTD},
                            {0b00001000, DI_04,&PINB, false, &PORTB}
};

portStruct doPorts[5] = {
                            {0b10000000, DO_00,&PORTD},
                            {0b00000001, DO_01,&PORTB},
                            {0b00000010, DO_02,&PORTB},
                            {0b00000100, DO_03,&PORTB},
                            {0b00010000, DO_04, &PORTB}
                        };
void setup()
{
    Serial.begin(9600);
    //DDRD = 0b11000010; //7,6,5,4,3,2,1,0
    //DDRB = DDRB | 0b00000111; //A7,A6,13,12,11,10,9,8
    DDRD = 0b11000010; //7,6,5,4,3,2,1,0
    DDRB = DDRB | 0b00010111; //A7,A6,13,12,11,10,9,8
    //This operation will not touch the pins we dont want and those pins will remain in theri old state.
    initServo();
    initPorts();
    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop()
{  
    //Reading Input Signals From Raspberry pi 
    servoFWD = readDIPort(DI_00);
    servoREV = readDIPort(DI_01);
    stepperFWD = readDIPort(DI_02);
    stepperREV = readDIPort(DI_03);

    //Reading Analog Sensors
     potOneVal = readPot(potOnePin);
     potTwoVal =  readPot(potTwoPin);
     tempC = readTemp(tempPin);
     vibrationValue = readVib(vibPin);
    
    if( (!servoFWD & !servoREV) || (servoFWD & servoREV) ) {
        stopServo(trolly);
    } else if ( servoFWD & !servoREV ) {
        fwdServo(trolly);
    } else if ( !servoFWD & servoREV ) {
       revServo(trolly); 
    } 
    if( (!stepperFWD & !stepperREV) || (stepperFWD & stepperREV) ) {
        stopStepper();
    } else if ( stepperFWD & !stepperREV ) {
        fwdStepper();
    } else if ( !stepperFWD & stepperREV ) {
       revStepper(); 
    }
    //delay(50);
    if(number)
        Serial.println(number);
    number = 0;
}

void initServo()
{
    trolly.attach(A3); // servo attached to Analog Pin 3
    trolly.write(90); // Initially the servo must be stopped 
}
void stopServo(Servo servo)
{
    servo.write(90); //Stop the Servo
    servo.detach(); // detach servo
}
void fwdServo(Servo servo)
{
    servo.attach(A3); // Attach servo
    servo.write(0);//rotate the Servo forward
}

void revServo(Servo servo)
{
    servo.attach(A3); //Attach servo
    servo.write(180); //rotate the Servo reverse
}

void stopStepper()
{
    writeDOPort(DO_00, false);
    writeDOPort(DO_01, false);
    writeDOPort(DO_02, false);
    writeDOPort(DO_03, false);
}

void fwdStepper()
{
    for(int i=0;i<200;i++){
        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);

        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);
    }
}

void revStepper()
{
    for(int i=0;i<200;i++){
        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);

        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);
    }
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


void writeDOPort(byte port,boolean value) 
{
    for(unsigned int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
        if( doPorts[i].pin == port){
                if(value) {
                    *doPorts[i].hwPort = *doPorts[i].hwPort | doPorts[i].bitMask;
                }
                else{
                    *doPorts[i].hwPort &= ~(doPorts[i].bitMask);
                } 
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

void receiveData(int byteCount)
{
    while(Wire.available()) {
        number = Wire.read();
    }
}

void sendData(){
    data[0] = (byte)potOneVal;
    data[1]= (byte)potTwoVal;
    data[2] = (byte)tempC;
    data[3] = (byte)vibrationValue;
    Wire.write(data,4);
 }


 int readTemp(byte tempPin)
{
     // put your main code here, to run repeatedly:
  float rawAdcVal = 0;
  float voltage = 0;
  float volInMilli = 0;
  float celcius = 0;
  float farenheit = 0;
  for(int i=1;i<10;i++){
    rawAdcVal += analogRead(tempPin);
    delay(1);
  }
  rawAdcVal /= 10.0;
  voltage = (5 * rawAdcVal) / 1024;
  //voltage = (aref_voltage * rawAdcVal) / 1024; //for 3.3v
  volInMilli = voltage * 1000;
  celcius = (volInMilli - 500) / 10; // 500 is offset for tmp36,10 is 10mv precision.
  farenheit = (celcius * 9.0 / 5.0) + 32.0;
  int val = (int)celcius;
  //Serial.print("celcius = ");
  //Serial.println(val);
    if( val < 128 )
        return val;
    else
        return valHigh;
}

 int readVib(byte vibPin)
 {
 
    // Read Piezo ADC value in, and convert it to a voltage
  float piezoADC = analogRead(vibPin);
  float piezoV = piezoADC / 1023.0 * 5.0;
  int value = (int)piezoV*10;
  //Serial.print("VibValue = ");
  //Serial.println(value);
  if( value < 128 )
    return value;
  else 
    return valHigh;
 }

 
int readPot(byte potPin){
    float rawV = analogRead(potPin);
    float potval= (rawV/1023.0)*127.0;
    int val = (int)potval;
    //Serial.print("PotValue at Analog");
    //Serial.print(potPin);
    //Serial.print("-->");
    //Serial.println((int)potval);
    if( val < 128 )
        return (int)potval;
    else
        return (int)valHigh;
 }
