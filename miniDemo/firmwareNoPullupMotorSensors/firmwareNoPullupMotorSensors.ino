#include<Servo.h>
#include<Wire.h>

#define SLAVE_ADDRESS 0x04

Servo trolly;

byte DI_00 = 4; //PD4,ServoFwd
byte DI_01 = 5; //PD5,ServoRev
byte servoPin = 3; //PD3,ServoRev

//Analog Ports
byte potOnePin = 0;// Analog 0 port
byte tempPin = 1; //Analog 1 Port
byte vibPin = 2; //Analog 2 Port

byte servoFWD = 0;
byte servoREV = 0;
byte potOneVal = 0;
byte tempVal = 0;
byte vibVal = 0;
byte i2cData[3] = {0,0,0};
byte valueLow = 0;



struct portStruct {
    byte bitMask;
    byte pin;
    volatile unsigned char *hwPort;
    boolean pullupFlag;
    volatile unsigned char *pullup;
};

portStruct diPorts[2] = {
                            {0b00010000, DI_00,&PIND, false,&PORTD},
                            {0b00100000, DI_01,&PIND, false,&PORTD}
};


void setup()
{
    DDRD |= 00001010; //7,6,5,4,3,2,1,0
    //Setting servoPin as Output, PD4 and PD5 as input.

    initPorts();
    initServo(trolly, servoPin);
    //Serial.begin(9600);
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendData);
    
    
}
void loop()
{
    
    //Reading Input Signals From Raspberry pi 
    servoFWD = readDIPort(DI_00);
    servoREV = readDIPort(DI_01);

    //Reading Analog Sensors
     potOneVal = readPot(potOnePin);
     tempVal = readTemp(tempPin);
     vibVal = readVib(vibPin);
     if( (!servoFWD & !servoREV) || (servoFWD & servoREV) ) {
        //Serial.println("Servo Stopped");
        stopServo(trolly);
    } else if ( servoFWD & !servoREV ) {
        //Serial.println("Servo in FWD");
        fwdServo(trolly,servoPin);
    } else if ( !servoFWD & servoREV ) {
       //Serial.println("Servo in REV");
       revServo(trolly, servoPin); 
    } 
    
    //revServo(trolly, servoPin);
}
void initPorts()
{
   for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        pinMode(diPorts[i].pin ,INPUT);
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

void initServo(Servo servo,byte ServoPin)
{
    servo.attach(ServoPin); // servo attached to Analog Pin 3
    servo.write(90); // Initially the servo must be stopped 
}
void stopServo(Servo servo)
{
    servo.write(90); //Stop the Servo
    servo.detach(); // detach servo
}
void fwdServo(Servo servo,byte ServoPin)
{
    servo.attach(ServoPin); // Attach servo
    servo.write(0);//rotate the Servo forward, 74, 77(ok)
}

void revServo(Servo servo, byte ServoPin)
{
    servo.attach(ServoPin); //Attach servo
    servo.write(180); //rotate the Servo reverse , 91 ,94,90(ok)
}

void sendData(){
    i2cData[0]  = potOneVal;
    i2cData[1] = tempVal;
    i2cData[2] = vibVal;
    Wire.write(i2cData,3);
   // Wire.write(55);
 
 }

byte readPot(byte potPin){
    float rawV = analogRead(potPin);
    float potval= (rawV/1023.0)*127.0;
    int val = (int)potval;
    //Serial.print("PotValue at Analog");
    //Serial.print(potPin);
    //Serial.print("-->");
    //Serial.println((int)potval);
    if( (val < 128) && (val >= 0))
        return (byte)val;
    else
        return valueLow;
 }

 byte readTemp(byte tempPin)
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
    if( (val < 128)  && (val >= 0))
        return (byte)val;
    else
        return valueLow;
}

 byte readVib(byte vibPin)
 {
 
    // Read Piezo ADC value in, and convert it to a voltage
  float piezoADC = analogRead(vibPin);
  float piezoV = piezoADC / 1023.0 * 5.0;
  int value = (int)piezoV*10;
  //Serial.print("VibValue = ");
  //Serial.println(value);
  if( (value < 128) && (value >=0))
    return value;
  else 
    return valueLow;
 }

