#define DI_00 36 //proximitySen_1
#define DI_01 37 //proximitySen_2
#define DI_02 38 //cameraSignal
#define DI_03 39
#define DO_00 40 //conveyor
#define DO_01 41 //crankShaft
#define DO_02 42 //stepperMotor
#define DO_03 43

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
     Serial.println("pin = " + String(diPin[i]));
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
    
    initPorts();
    Serial.begin(9600);
    check(0b00000001);
}

void loop()
{
    
}
void check( byte f)
{
    for( int i= 0 ; i< sizeof(diPorts)/sizeof(portStruct);i++) {
         byte a = ( (f & diPorts[i].bitMask[i]) );
         switch( (a) )
         {
         case 1:
            Serial.println(diPorts[i].pin);
             diPorts[i].curState = true;
             Serial.println("curState = " + String(diPorts[i].curState));
             if( diPorts[i].curState != diPorts[i].prevState ){
//                 publish(diPorts[i].portName, 
//                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(    diPorts[i].curState    ) + "published");  
             }    
           diPorts[i].prevState = diPorts[i].curState;
             break;
         case 2:
            Serial.println(diPorts[i].pin);
            diPorts[i].curState = true;
            Serial.println("curState = " + String(diPorts[i].curState));
            if( diPorts[i].curState != diPorts[i].prevState ){
//                 publish(diPorts[i].portName, 
//                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(    diPorts[i].curState   ) + "published");  
             }    
           diPorts[i].prevState = diPorts[i].curState;
            
            break;
         case 4: 
            Serial.println(diPorts[i].pin);
            diPorts[i].curState = true;
            Serial.println("curState = " + String(diPorts[i].curState));
            if( diPorts[i].curState != diPorts[i].prevState ){
//                 publish(diPorts[i].portName, 
//                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(    diPorts[i].curState    ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
         case 8: 
            Serial.println(diPorts[i].pin);
            diPorts[i].curState = true;
            Serial.println("curState = " + String(diPorts[i].curState));
            if( diPorts[i].curState != diPorts[i].prevState ){
//                 publish(diPorts[i].portName, 
//                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(     diPorts[i].curState    ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
         default:
            break;           
        
    }
}
}
