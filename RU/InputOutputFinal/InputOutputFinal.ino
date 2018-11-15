#include<SPI.h>
#include<Ethernet.h>
#include<PubSubClient.h>

#define DI_00  37 //proximitySen_1
#define DI_01  36 //proximitySen_2
#define DI_02  35 //cameraSignal
#define DI_03  34
#define DO_00  33 //conveyor
#define DO_01  32 //crankShaft
#define DO_02  31 //stepperMotor
#define DO_03  30

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

void initPorts()
{
    
  for( int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
     diPorts[i].portName = ruidStr + "/DI_0"+ i;
     pinMode(diPorts[i].pin ,INPUT);
     Serial.println("inputPin = " + String(i) + "/" +  String(diPorts[i].pin));
  }

  for( int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
     doPorts[i].portName = ruidStr + "/DO_0"+ i;
     pinMode(doPorts[i].pin ,OUTPUT);
     Serial.println("outputPin = " + String(i) + "/" +  String(doPorts[i].pin));
     Serial.println(doPorts[i].portName + "   subscribed");
     subscribe(doPorts[i].portName);
  }
}
void setup()
{
    Serial.begin(9600);
    DDRC = 0b11110000; //30,31,32,33, 34,35,36,37
    initPorts();
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
        //Read DI ports
        readDIPorts();
        delay(10);
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
            doPorts[i].prevState = doPorts[i].curState;
            if ( (String)messageBuff == "true")  {
                 doPorts[i].curState = true;
            } else {
                 doPorts[i].curState = false;
              }
           
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
            publish(diPorts[i].portName, 
                ((val == true)? "true":"false"),false);
            Serial.println(diPorts[i].portName+ String(     val    ) + "published");  
        }
        diPorts[i].prevState = diPorts[i].curState;
     }
}

