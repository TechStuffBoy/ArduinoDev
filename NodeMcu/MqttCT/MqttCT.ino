#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>
char messageBuff[1000];
//const char* ssid = "KKNhome";
const char* ssid = "CPSgateway-wifi";
const char* password = "DhashanTech";

//byte server[] = {192, 168, 0, 14};
byte server[] = {192, 168, 3, 8};
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "sensors/test/temperature";

String topicCallback;
boolean messageReceived = false;

const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time
const int adc_zero = 493;                                                     // relative digital zero of the arudino input from ACS712 (could make this a variable and auto-adjust it)
const int currentPin = A0;
#define light D0
String readCurrentSensor = "";

WiFiClient espClient;
PubSubClient client(server,1883,callback,espClient);

void setup()
{
    Serial.begin(115200);
    setupWifi();
    client.connect("ESP8266Client", "hello", 1, true, "unexpected Disconnect");
    subscribe("status/CT");
}
void loop()
{
   readCurrentSensor = readCT();
   if(messageReceived == true) {
        messageReceived = false;
        if( (topicCallback == "status/CT") && ( (String)messageBuff == "status" )) {
                publish("status/CT",readCurrentSensor,false);
                 myDelay(2);
        }
   }
    Serial.println(readCurrentSensor);
    if (!client.connected()) {
        reconnect();
        subscribe("status/CT");
    }
    //publish("NodeMcu","Hello,World!",false);
    myDelay(2);
    client.loop();
}


void setupWifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  topicCallback = topic;
  Serial.print("] ");
  int i=0;
  for (i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
     messageBuff[i]=payload[i];
  }
  messageBuff[i]='\0';
  Serial.println();
  messageReceived = true;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", "hello", 1, true, "unexpected Disconnect")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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

void myDelay(unsigned long sec)
{
    unsigned long delayMSec = sec * 1000;
    unsigned long recordMSec = millis();
    messageReceived = false;
    //while loop will run untill intervel time,then it will comeout
    while(((millis() - recordMSec) < delayMSec) && (!messageReceived)) {
        client.loop();
    }
}

float readCT(){
     unsigned long currentAcc = 0;
    unsigned int count = 0;
    int sensorValue = 0;
    for (count = 0; count < numSamples; count++) {
        int adc_raw = analogRead(currentPin)- adc_zero;
        currentAcc += (unsigned long)(adc_raw * adc_raw);
        delayMicroseconds(sampleInterval);
    }
   
   //The 50 for the conversion comes from solving for X, 5V / X = .100 V / 1 A
   //0.185 for 5A model, 0.100 For 20A Model, 0.066 for 30A Model 
   
    //float rms = sqrt((float)currentAcc/(float)numSamples) * (92.593 / 1000.0); // For WCS2720
    //float rms = sqrt((float)currentAcc/(float)numSamples) * (75.7575 / 1024.0); // For 30A Model
    //float rms = sqrt((float)currentAcc/(float)numSamples) * (50 / 1024.0); // For 20A Model
    float rms = sqrt((float)currentAcc/(float)numSamples) * (27 / 1024.0); // For 5A Model

    //Serial.println(rms);
    //Serial.println(analogRead(currentPin));
    
    
    float rmsAcc = rms - 0.04; //subtract the stray value, It Will change for sensor to sensors.
    if((rmsAcc > 0)) {
        return rmsAcc;
    } else 
        return (float)0;
}

