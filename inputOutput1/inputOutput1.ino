#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>

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

byte mac[] = {0x11, 0x11, 0x11, 0x00, 0x00, 0x01};

byte server[] = {192, 168, 3, 14};
byte ip[] = {192, 168, 3, 34};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 3, 1};


EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

String ruidStr = "RU_01";
String myMacId = "11-11-11-00-00-01";
String clientId = "RU_" + myMacId;
String clientState = "client/state/RU_" + myMacId;
String topicCallback;

//char Buffers
char clientIdBuff[50];
char messageBuff[400];

boolean messageReceived = false;

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
     subscribe(doPorts[i].portName);
  }
}
void setup()
{
    Serial.begin(9600);
    DDRC = 0b11110000; //30,31,32,33, 34,35,36,37
    Ethernet.begin(mac, ip, gateway, subnet);
    Serial.println("Ethernet Initiated");
    clientId.toCharArray(clientIdBuff, (clientId.length() + 1));
}

void loop()
{
     if (!client.connected()) {
        Serial.println("Trying to connect to MQTT Broker...");
        client.connect(clientIdBuff, clientIdBuff, 1, true, "Unexpected Disconnect");
        if(client.connected()) {
            Serial.println ("Success!! MQTT:" + myMacId + " Connected! ");
            publish(clientState, clientId, true); //also send retainflag = true
            initPorts();
        } else {
            delay(10000);
        }
    } else {
        //Serial.println("Client Connected");
        //Read DI ports
        //checkDiPort();
        check();
        delay(100);
    }
     client.loop();
}


void callback(char* topic, byte* payload, unsigned int length) 
{
    int i=0;
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.println("] ");
    topicCallback = topic;
    for( i=0;i<length;i++){
        messageBuff[i]=payload[i];
    }
    // Hav to stop the string so giving \0
    messageBuff[i]='\0';
    Serial.print("messageBuff:");
    Serial.println(messageBuff);
    Serial.println();
    messageReceived = true;
    for( int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
        if( topicCallback == doPorts[i].portName ){
            if ( (String)messageBuff == "true")  {
                 doPorts[i].curState = true;
            } else {
                 doPorts[i].curState = false;
              }
           doPorts[i].prevState = doPorts[i].curState;
           if ( doPorts[i].curState != doPorts[i].prevState ) {
             digitalWrite( doPorts[i].pin , doPorts[i].curState );
             Serial.println( "value of" + doPorts[i].portName + (String)doPorts[i].curState );
           }
        }
    }
}
void publish(String topic, String msg, boolean retainFlag)
{
    char topicBuf[100];
    char msgBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    msg.toCharArray(msgBuf, msg.length()+1);
    client.publish(topicBuf, msgBuf, retainFlag);
}

void subscribe(String topic) {
    char topicBuf[100];
    topic.toCharArray(topicBuf, topic.length() + 1);
    client.subscribe(topicBuf);   
}

void myDelay(unsigned long sec)
{
    unsigned long delayMSec = sec * 1000;
    unsigned long recordMSec = millis();
    messageReceived = false;
    while(((millis() - recordMSec) < delayMSec) && (!messageReceived)) {
        client.loop();
    }
}

void checkDiPort()
{
       byte recd;
       boolean val;
     for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        recd = digitalRead( diPorts[i].pin);
        val = ((recd == 0)? false:true);
        diPorts[i].curState = val;
        //Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
        
        if( val != diPorts[i].prevState ){
            publish(diPorts[i].portName, 
                ((val == true)? "true":"false"),false);
            Serial.println(diPorts[i].portName+ String(     val    ) + "published");  
        }
        diPorts[i].prevState = diPorts[i].curState;
     }
}

void check()
{
    byte recd;
    boolean val;
    for( int i= 0 ; i< sizeof(diPorts)/sizeof(portStruct);i++) {
         recd = PINC;
         byte a = ( (recd & diPorts[i].bitMask[i]) );
         val = ((recd == 0)? false:true);
         diPorts[i].curState = val;
         Serial.println("Curval =" + String ( diPorts[i].curState ));
         Serial.println("I am in for loop");
         switch( (a) )
         {
         case 1:
          Serial.println("I am in case1");
             diPorts[i].prevState = false;
             if( diPorts[i].curState != diPorts[i].prevState ){
                 publish(diPorts[i].portName, 
                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(   val    ) + "published");  
             }    
           diPorts[i].prevState = diPorts[i].curState;
             break;
         case 2:
         Serial.println("I am in case2");
         diPorts[i].prevState = false;
            if( diPorts[i].curState != diPorts[i].prevState ){
                 publish(diPorts[i].portName, 
                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(    val   ) + "published");  
             }    
           diPorts[i].prevState = diPorts[i].curState;
            
            break;
         case 4: 
         Serial.println("I am in case3");
         diPorts[i].prevState = false;
            if( diPorts[i].curState != diPorts[i].prevState ){
                 publish(diPorts[i].portName, 
                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(   val    ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
         case 8: 
         Serial.println("I am in case4");
         diPorts[i].prevState = false;
            if( diPorts[i].curState != diPorts[i].prevState ){
                 publish(diPorts[i].portName, 
                    ((val == true)? "true":"false"),false);
                 Serial.println(diPorts[i].portName+ String(     val    ) + "published");  
             }    
            diPorts[i].prevState = diPorts[i].curState;
            break;
          default:
            break;           
        
    }
}
}
