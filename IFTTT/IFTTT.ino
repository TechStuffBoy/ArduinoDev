#include <SPI.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>

//InputOutputPorts
#define DI_00  37 //proximitySen_1
#define DI_01  36 //proximitySen_2
#define DI_02  35 //cameraSignal
#define DI_03  34
#define DO_00  33 //conveyor
#define DO_01  32 //crankShaft
#define DO_02  31 //stepperMotor
#define DO_03  30

String compLHSGlobal ="";
String compRHSGlobal = "";
String compSYMGlobal = "";
String actionLHSGlobal = "";
String actionRHSGlobal = "";
String ruidStr = "RU_01";
int counter[4] = {0,0,0,0};

char messageBuff[1000];
String topicCallback;
boolean messageReceived = false;
int whichDiPort = 0;
int whichDoPort = 0;
byte mac[] = {0x00, 0x11, 0x11, 0x00, 0x00, 0x01};
byte server[] = {192, 168, 3, 2};
byte ip[] = {192, 168, 3, 3};
byte subnet[] = {255, 255, 255, 0};
byte gateway[] = {192, 168, 3, 1};

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

char arrayNum[10];
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
	initPorts();
	SPI.begin();
	Ethernet.begin(mac, ip, gateway, subnet);
	client.connect("RU_01", "will_RU", 0, false, "false");
	if(client.connected()) {
		Serial.println("Client connected to Broker");
		subscribe("IFTT");
	} else {
		client.connect("RU_01", "will_RU", 0, false, "false");
		delay(5000);
	}
	initPorts();
}

void loop()
{
	if( !(client.connected()) ) {
		client.connect("RU_01", "will_RU", 0, false, "false");
		//subscribe("IFTT");
	}
	readDIPorts();
	checkIFTTT();
}

void callback(char* topic, byte* payload, unsigned int length)
{
	unsigned int i=0;
	Serial.print(F("Message arrived ["));
	Serial.print(topic);
	Serial.println(F("] "));
	topicCallback = topic;
	for( i=0;i<length;i++){
		messageBuff[i]=payload[i];
	}
	// Have to stop the string so giving \0
	messageBuff[i]='\0';
	Serial.print(F("messageBuff:"));
	Serial.println(messageBuff);
	Serial.println();
	messageReceived = true;
	StaticJsonBuffer<450> jsonBuffer;
	if(topicCallback == "IFTT") {
		JsonObject& root = jsonBuffer.parseObject(messageBuff);
		const char* compLHS = root ["RU_01/IFTTT"]["compLHS"];
		const char* compSYM = root ["RU_01/IFTTT"]["compSYM"];
		const char* compRHS = root ["RU_01/IFTTT"]["compRHS"];
		const char* actionRHS = root ["RU_01/IFTTT"]["actionRHS"];
		const char* actionLHS = root ["RU_01/IFTTT"]["actionLHS"];
		compLHSGlobal = compLHS;
		compRHSGlobal = compRHS;
		compSYMGlobal = compSYM;
		actionLHSGlobal = actionLHS;
		actionRHSGlobal = actionRHS;
	}
		
}

int getInputValue(String value)
{
	//Analog Input
	if(value.substring(0, 2) == "AI_") {
		int symbolIndex = value.indexOf('_');
		String firstValue = value.substring(symbolIndex+1);
		int analogPort = firstValue.toInt();
		if(analogPort == 0) {
			return  analogRead(A11); //change pin where sensor is connected
		}
	}
	else if(value.substring(0, 2) == "AR_") {
		// Do something
		}  else {
		return value.toInt();
	}
	
	// Digital Input
	if(value.substring(0, 2) == "DI_") {
		int symbolIndex = value.indexOf('_');
		String firstValue = value.substring(symbolIndex+1);
		return (int)diPorts[firstValue.toInt()].curState;
	}
	else if(value == "true") {
		return 1;
	}
	else if (value == "false") {
		return 0;
	}
	else if(value.substring(0, 2) == "DR_") {
		//do something
	}
	
	return 0;
}

void setOutput(String port, String value)
{
	boolean pinState = false;
	String firstValue;
	int symbolIndex = 0;
	if(port.substring(0, 2) == "DO_") {
		symbolIndex = port.indexOf('_');
		firstValue = port.substring(symbolIndex+1);
		//diPorts[firstValue.toInt()].pin;
	}
    if( value == "true") {
		pinState = true;
	} else if (value == "false") {
		pinState = false;
	} else if(value.substring(0, 2) == "DI_") {
		int symbolIndex = value.indexOf('_');
		String firstValue = value.substring(symbolIndex+1);
		pinState = diPorts[firstValue.toInt()].curState;
      }
	digitalWrite(doPorts[firstValue.toInt()].pin , pinState);
}
void checkIFTTT() 
{
	if (compSYMGlobal.equals("=="))  {
	
		if(getInputValue(compLHSGlobal) == getInputValue(compRHSGlobal)) {
			setOutput(actionLHSGlobal,actionRHSGlobal);
		}
			
	} 
	else if (compSYMGlobal.equals(">=")) {
		if(getInputValue(compLHSGlobal) >= getInputValue(compRHSGlobal)) {
			setOutput(actionLHSGlobal,actionRHSGlobal);
	    }
     } 
	else if(compSYMGlobal.equals("<=")) {
		if(getInputValue(compLHSGlobal) <= getInputValue(compRHSGlobal)) {
			setOutput(actionLHSGlobal,actionRHSGlobal);
		} 
	 }
}

void subscribe(String topic) {
	char topicBuf[100];
	Serial.println("Subscribe to " + topic);
	topic.toCharArray(topicBuf, topic.length() + 1);
	client.subscribe(topicBuf);
}

void unsubscribe(String topic) {
	char topicBuf[100];
	Serial.println("unsubscribe to " + topic);
	topic.toCharArray(topicBuf, topic.length() + 1);
	client.unsubscribe(topicBuf);
}

void publish(String topic, String msg, boolean retainFlag)
{
	char topicBuf[100];
	char msgBuf[400];
	Serial.println("Publish: Topic = " + topic + " Msg = " + msg);
	topic.toCharArray(topicBuf, topic.length() + 1);
	msg.toCharArray(msgBuf, msg.length()+1);
	client.publish(topicBuf, msgBuf, retainFlag);
}
void initPorts()
{
	/*
	errorSAPStr = ruidStr + "/SAP_ERROR";
	rfidSubscribeStr = ruidStr + "/OP_AUTH_RESP";
	getWolStr = ruidStr + "/GET_WOL";
	setRmlStr = ruidStr +  "/SET_RML";
	getRmlStr = ruidStr +  "/GET_RML";
	setWolStr = ruidStr + "/SET_WOL";
	getProdOrderDetailStr = ruidStr + "/GET_POD";
	productionStopStr = ruidStr + "/END_PO";
	productionStartStr = ruidStr  + "/START_PO";
	*/
	for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
		diPorts[i].portName = ruidStr + "/DI_0"+ i;
		pinMode(diPorts[i].pin ,INPUT);
		//Serial.println("inputPin = " + String(i) + "/" +  String(diPorts[i].pin));
	}

	for( unsigned int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
		doPorts[i].portName = ruidStr + "/DO_0"+ i;
		pinMode(doPorts[i].pin ,OUTPUT);
		//Serial.println("outputPin = " + String(i) + "/" +  String(doPorts[i].pin));
		//Serial.println(doPorts[i].portName + "   subscribed");
		subscribe(doPorts[i].portName);
	}
}

void readDIPorts()
{
	boolean val;
	byte recd = PINC;
	//Serial.println("recd = " + String(recd));
	for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
		val = (((recd & diPorts[i].bitMask) == 0)? false:true);
		diPorts[i].curState = val;
		//Serial.println( diPorts[i].portName +  "Val  " + String(val) + " i = " + i + " curState = " + String(diPorts[i].curState) + " prevState = " + String(diPorts[i].prevState));
		
		if( val != diPorts[i].prevState ){
			if( val == true) {
				counter[i] += 1;
				Serial.println("Counter of  " + String(i) + "   " +  String(counter[i]));
			}
			publish(diPorts[i].portName,
			((val == true)? "true":"false"),false);
			Serial.println(diPorts[i].portName+ String(     val    ) + "published");
		}
		diPorts[i].prevState = diPorts[i].curState;
	}
}


/*
void checkIFTTT()
{       
		boolean valLHS = false;
		boolean valRHS = false;
		byte recd = PINC;
		switch(compSYMGlobal) 
		{
		case "==" :
		   if (compLHSGlobal.equals(getDiValue(compLHSGlobal))) {
			 valLHS = (((recd & diPorts[whichDiPort].bitMask) == 0)? false:true);
			 //(valLHS == true)? "true":"false";
			
		   }
		   
		   if (compRHSGlobal.equals(getDiValue(compRHSGlobal))) {
				valRHS = (((recd & diPorts[whichDiPort].bitMask) == 0)? false:true);
				//(valRHS == true)? "true":"false";
		   }
		   
		   if (valLHS == compLHSGlobal.toInt()) {
			   char buff[10];
			   Serial.println("Fire alarm is ON");
			   digitalWrite( actionLHSGlobal.toCharArray(buff ,10) , actionRHSGlobal.toInt());
		   }
			
		   if (valRHS == compRHSGlobal.toInt()) {
				 char buff[10];
				 Serial.println("Fire alarm is ON");
				 digitalWrite( actionLHSGlobal.toCharArray(buff,10) , actionRHSGlobal.toInt());
		   }
		    if (valLHS == valRHS) {
			   // do something
		    }
			break;
		}
		  
			   
		   
		   
		   
			//if(DI_00 == DI_01)
			if(getDIvalue(compLHSGlobal) == GatewayIDR)
			if(getDIvalue(compLHSGlobal) == True/False)
			if(getDIvalue(compLHSGlobal) == getDIvalue(compRHSGlobal))
			{
				DO_00 = true;
				setDOvalue(actionLHSGlobal, )
			}
		
		if (compLHSGlobal.equals("DI_00")) && (compSYMGlobal.equals("==") ){
			val = (((recd & diPorts[0].bitMask) == 0)? false:true);
			(val == true)? "true":"false";
			if ( (val == true)? "true":"false" == compRHSGlobal ) {
				Serial.println("Fire alarm is ON");
				digitalWrite( DO_03 , HIGH );
			}
		}  
} 



boolean getDiValue(String value)
{
	if(value.substring(0, 2) == "DI_") {
		int symbolIndex = value.indexOf('_');
		String firstValue = value.substring(symbolIndex+1);
		return diPorts[firstValue.toInt()].curState;
	}
	else if(value == "true") {
		return true;
	}
	else if (value == "false") {
		return false;
	}
	else if(value.substring(0, 2) == "DR_") {
		//do something
	}
	
	return false;
}

int getAnalogValue(String value)
{
	if(value.substring(0, 2) == "AI_") {
		int symbolIndex = value.indexOf('_');
		String firstValue = value.substring(symbolIndex+1);
		int analogPort = firstValue.toInt();
		if(analogPort == 0) {
			return  analogRead(A11);
		}
	}
	else if(value.substring(0, 2) == "AR_") {
		// Do something
		}  else {
		return value.toInt();
	}
	return 0;
}
	if ( getDiValue(compLHSGlobal)) {
			parseOutput(actionLHSGlobal,actionRHSGlobal);
			//Turn on/off Some Output
		} else if ( !getDiValue(compLHSGlobal)) {
			parseOutput(actionLHSGlobal,actionRHSGlobal);
			//Turn on/off Some Output
	    }
		else if ( getDiValue(compRHSGlobal) ) {
			//Turn on/off Some Output
			parseOutput(actionLHSGlobal,actionRHSGlobal);
		}
		else  if (getDiValue(compLHSGlobal) == getDiValue(compRHSGlobal)) {
			//Do something
			parseOutput(actionLHSGlobal,actionRHSGlobal);
		}
		else if(getAnalogValue(compLHSGlobal) == getAnalogValue(compRHSGlobal)) {
			//Do something
			parseOutput(actionLHSGlobal,actionRHSGlobal);
		}
		*/
