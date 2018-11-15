

#define DI_00 34 //proximitySen_1
#define DI_01 35 //proximitySen_2
#define DI_02 36 //cameraSignal
#define DI_03 37
#define DO_00 30 //conveyor
#define DO_01 31 //crankShaft
#define DO_02 32 //stepperMotor
#define DO_03 33

byte myVar;
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
String ruidStr = "RU_01";
portStruct diPorts[4];
portStruct doPorts[4];

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


void setup()
{
    DDRC = 0b11110000; //30,31,32,33, 34,35,36,37
    myVar = PINC;
    initPorts();
    Serial.begin(9600);
   
}

void loop()
{
    //myVar = PINC;
    //Serial.println((String)myVar);
    check();
    delay(50);
}
void check()
{
    byte recd;
    boolean val;
   // recd = PINC;
    for( int i= 0 ; i< sizeof(diPorts)/sizeof(portStruct);i++) {
         recd = PINC;
         byte a = ( (recd & diPorts[i].bitMask[i]) );
         val = ((recd == 0)? false:true);
         switch(a)
         {
         case 1:
             diPorts[i].curState = val;
             if( diPorts[i].curState != diPorts[i].prevState ){
                 Serial.println(diPorts[i].portName+ String(    diPorts[i].curState    ) + "published");  
             }    
             diPorts[i].prevState = diPorts[i].curState;
             break;
         case 2:
            diPorts[i].curState = val;
            if( diPorts[i].curState != diPorts[i].prevState ){
                 Serial.println(diPorts[i].portName+ String(    diPorts[i].curState   ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
         case 4: 
            diPorts[i].curState = val;
            if( diPorts[i].curState != diPorts[i].prevState ){
                 Serial.println(diPorts[i].portName+ String(    diPorts[i].curState    ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
         case 8: 
            diPorts[i].curState = val;
            if( diPorts[i].curState != diPorts[i].prevState ){
                 Serial.println(diPorts[i].portName+ String(     diPorts[i].curState    ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
          default:
            break;           
        
    }
}
}
