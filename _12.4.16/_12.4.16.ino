
#define DI_00 34 //proximitySen_1
#define DI_01 35 //proximitySen_2
#define DI_02 36 //cameraSignal
#define DI_03 37
#define DO_00 30 //conveyor
#define DO_01 31 //crankShaft
#define DO_02 32 //stepperMotor
#define DO_03 33

byte diPin[4] = {DI_00, DI_01, DI_02, DI_03};
byte doPin[4] = {DO_00, DO_01, DO_02, DO_03};
struct portStruct {
    byte bitMask[8] = { 0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000};
    String portName;
    byte pin;
    byte portNo;
    boolean curState;
    boolean prevState;
    
};

portStruct diPorts[4];
portStruct doPorts[4];
String ruidStr = "RU_01";

void setup()
{
     DDRC = 0b00000011;
     initPorts();
}

void loop()
{

    checkDiPort();
    delay(1000);
}
 /* if (PINC & (1 << PINC0)){
       curState1 = true;
 } 
if ( PINC & (1 << PINC1)) {
    curState2 = true;
 } 
 if ( PINC & (1 << PINC2)) {
    curState3 = true;
 } 
if (PINC & (1 << PINC3 )) {
    curState4 = true;
 }  
}
*/
void initPorts()
{
 for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
     diPorts[i].portName = ruidStr + "/DI_0"+ i;
     diPorts[i].pin =  diPin[i];
     diPorts[i].portNo = i;
     diPorts[i].curState = false;
     diPorts[i].prevState = false;
     pinMode(diPin[i] ,INPUT);
     //Serial.println("pin = " + String(diPin[i]));
  }

  for( int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
     doPorts[i].portName = ruidStr + "/DO_0"+ i;
     doPorts[i].pin =  doPin[i];
     doPorts[i].portNo = i;
     doPorts[i].curState = false;
     doPorts[i].prevState = false;
     pinMode(doPin[i] ,OUTPUT);
     //subscribe(doPorts[i].portName);
  }
}
void checkDiPort()
{
       byte recd;
       byte h;
       boolean val;
     for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        recd = PINC;
        h =  ( (recd & diPorts[i].bitMask[i]) );
         int x = h & (1 << i);
        //int x = h & (1 << i);
        Serial.println( x , BIN);
//        val = ((recd == 0)? false:true);
//        diPorts[i].curState = val;
//        //Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
//        
//        if( val != diPorts[i].prevState ){
//            publish(diPorts[i].portName, 
//                ((val == true)? "true":"false"),false);
//            Serial.println(diPorts[i].portName+ String(     val    ) + "published");  
//        }
//        diPorts[i].prevState = diPorts[i].curState;
     }
}
