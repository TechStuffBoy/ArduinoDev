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
 //I2C ErrorCodes
#define OWN_SLAVE_ADDRESS_PLUS_WRITE_CAME_FRM_MASTER_ACK_RETRND_BACK 0x60
#define ARBITRATION_LOST_IN_SLAVE_R_W_OWN_SL_ADDRS_RECVD_ACK_RETRND 0x68
#define GENERAL_CALL_ADDRESS_RECIEVED_ACK_RTRND 0x70
#define ARBITRATION_LOST_IN_SLAVE_R_W_GENERAL_CALL_ADDRESS_RECIEVED_ACK_RETRND 0x78
#define PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_ACK_RTRND 0x80
#define PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_NACK_RTRND 0x88
#define PREVIOUSLY_ADDRSED_WITH_GENERAL_CALL_DATA_RECVD_ACK_RTRND 0x90
#define PREVIOUSLY_ADDRSED_WITH_GENERAL_CALL_DATA_RECVD_NACK_RTRND 0x98
#define STOP_OR_REPEATED_START_RECVD_WHILE_STILL_ADDRSD_AS_SLAVE 0xA0

#define OWN_SL_ADDRS_PLUS_READ_RECVD_ACK_RTRND 0xA8
#define ARBTRN_LOST_IN_SL_ADDR_R_W_OWN_SL_ADDR_R_RECVD_ACK_RETRND 0xB0
#define DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED 0xB8
#define DATA_BYTE_IN_TWDR_TXMTD_NACK_RECIVED 0xC0
#define LAST_DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED 0xC8

#define NO_RELEVANT_STATE_INFO_AVAILABLE_TWINT_IS_ZERO 0xF8
#define BUS_ERROR_DUE_TO_ILLEGAL_START_STOP 0x00
#include<Servo.h>

void initSlave(void);
void masterWantsToReadSlave(void);
void slaveWriteOneByteBackToMaster(unsigned char write_data);
void masterWantsToWriteSlave(void);
void masterThenWritesByteToSlave(void);


#define SLAVE_ADDRESS 0x04

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
unsigned char write_data,recv_data;
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
    initSlave(SLAVE_ADDRESS); // Function to initilaize slave
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
     sendSensorData();
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

 void initSlave(unsigned char address ) // Function to initilaize slave
{
    Serial.println("Initializing the addressed slave at 100 KHz");
    //slave is initialized at 100Khz speed
    //TWSR = 0x00;
    //TWBR = 0x48;
    TWCR = (1<<TWEN);
    TWCR = (1<<TWINT);
    TWAR= ( address<<1 | 1); // Fill slave address to TWAR
}

void masterWantsToReadSlave(void) //Function to match the slave address and slave dirction bit(read) 
{
    Serial.println("Checking master Wants To Read Slave or Not");
    while((TWSR & 0xF8)!= OWN_SL_ADDRS_PLUS_READ_RECVD_ACK_RTRND ) // Loop till correct acknoledgement have been received
    {
        //printStatus(TWSR & 0xF8);
        Serial.println(TWSR & 0xF8,HEX);
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
        while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
    Serial.println("Verifying that master Wants To Read Slave");
}

 
void slaveWriteOneByteBackToMaster(unsigned char write_data) // Function to write data
{
    Serial.print("Writing Data ");
    Serial.print(write_data);
    Serial.println(" from slave to Master");
    TWDR= write_data;// Fill TWDR register whith the data to be sent 
    TWCR= (1<<TWEN)|(1<<TWINT)|(1<<TWEA);// Enable TWI, Clear TWI interrupt flag
    //printStatus(TWSR & 0xF8);
    Serial.println(TWSR & 0xF8,HEX);
    while((TWSR & 0xF8) != DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED); // Wait for the acknowledgement
    Serial.print("Writing ");
    Serial.print(write_data);
    Serial.println(" Success :)");
}

 

void slaveWriteLastByteBackToMaster(unsigned char write_data) // Function to write data
{
    Serial.print("Writing Data ");
    Serial.print(write_data);
    Serial.println(" from slave to Master");
    TWDR= write_data;           // Fill TWDR register whith the data to be sent 
    TWCR= (1<<TWEN)|(1<<TWINT);// Enable TWI, Clear TWI interrupt flag
    //printStatus(TWSR & 0xF8);
    Serial.println(TWSR & 0xF8,HEX);
    while((TWSR & 0xF8) != DATA_BYTE_IN_TWDR_TXMTD_NACK_RECIVED); // Wait for the acknowledgement
    Serial.print("Writing ");
    Serial.print(write_data);
    Serial.println(" Success :)");
}
void masterWantsToWriteSlave(void) //Function to match the slave address and slave dirction bit(read)
{
    Serial.println("Checking that the master Wants To Write Slave");
    while((TWSR & 0xF8)!= OWN_SLAVE_ADDRESS_PLUS_WRITE_CAME_FRM_MASTER_ACK_RETRND_BACK)  // Loop till correct acknoledgement have been received
    {
        //printStatus(TWSR & 0xF8);
        Serial.println(TWSR & 0xF8,HEX);
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
        while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
    Serial.println("Verifying that the master Wants To Write Slave");
}

void masterThenWritesByteToSlave(void)
{
    Serial.println("Slave tries to get one byte data from Master");
    // Clear TWI interrupt flag,Get acknowlegement, Enable TWI
    TWCR|=(1<<TWINT);
    TWCR|=(1<<TWEN)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO); 
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
    //printStatus(TWSR & 0xF8);
    Serial.println(TWSR & 0xF8,HEX);
    while((TWSR & 0xF8)!= PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_ACK_RTRND); // Wait for acknowledgement
    recv_data=TWDR; // Get value from TWDR
    Serial.print("Slave finally recieved ");
    Serial.print(recv_data);
    Serial.println("From the master");
}

void sendSensorData(){
    //data[0] = (byte)potOneVal;
    //data[1]= (byte)potTwoVal;
    //data[2] = (byte)tempC;
    //data[3] = (byte)vibrationValue;
    masterWantsToReadSlave();
    slaveWriteOneByteBackToMaster((byte)potOneVal);
    slaveWriteOneByteBackToMaster((byte)potTwoVal);
    slaveWriteOneByteBackToMaster((byte)tempC);
    slaveWriteLastByteBackToMaster((byte)vibrationValue);
    delay(40);   
 }

