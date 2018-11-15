#include <ESP8266WiFi.h>  
 // WiFi parameters  
 const char* ssid = "CPSgateway-wifi";  
 const char* password = "DhashanTech";  
 void setup(void)  
 {  
  // Start Serial  
  Serial.begin(115200);  
  delay(500);  
  // Connect to WiFi  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED)  
  {
   Serial.println("In wifi loop");  
   delay(500);  
  }  
  Serial.println("");  
  Serial.println("WiFi connected");  
  // Print the IP address  
  Serial.println(WiFi.localIP());  
  pinMode(LED_BUILTIN, OUTPUT);  
 }  
 void loop()  
 {  
  digitalWrite(LED_BUILTIN, LOW);  
  delay(1000);  
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(1000);  
 } 
