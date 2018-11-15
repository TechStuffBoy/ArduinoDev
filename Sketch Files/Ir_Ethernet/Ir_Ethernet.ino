#include<Wire.h>
#include<SPI.h>
#include<Ethernet.h>
#define DS1307_I2C_ADDRESS 0x68
unsigned long previousMillis=0;
unsigned long currentMillis;
unsigned int interval=1500;
unsigned int total=0;
unsigned int prev=0;
boolean currentLineIsBlank=true;

int sen1=5;
int sen2=6;
int val_1=0;
int val_2=0;
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress ip(192, 168, 1, 17);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);
EthernetClient client;
byte decToBcd(byte val)
{
 return (((val / 10) << 4) | (val % 10));
}
byte bcdToDec(byte val)
{
   return (((val & 0xF0) >> 4) * 10) + (val & 0x0F);
}

void readDS1307time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
} 

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS1307time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
 // send it to the serial monitor
  Serial.print(hour, DEC);
  client.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  client.print(":");
  if (minute<10)
  {
    Serial.print("0");
    client.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  client.print(minute, DEC);
  client.print(":");
  if (second<10)
  {
    Serial.print("0");
    client.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");

  client.print(second, DEC);
  client.print(" ");
  client.print(dayOfMonth, DEC);
  client.print("/");
  client.print(month, DEC);
  client.print("/");
  client.print(year, DEC);
  client.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    client.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    client.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    client.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    client.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    client.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    client.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    client.println("Saturday");
    break;
  }
 }
 
void actual_strength()
{
  Serial.print(" Total Strength :");
  Serial.println(total);
}

void object_counter()
{
  val_1=digitalRead(sen1);
  val_2=digitalRead(sen2);
  if((val_1==HIGH && val_2==LOW))
  {
      currentMillis=millis();     
      previousMillis=currentMillis;
      prev=0;
      prev=total;
      Serial.println("In in_condition");
  while(val_2 != HIGH)
   { 
    currentMillis=millis();
    if( (currentMillis-previousMillis) >=interval)
      {
        Serial.println("Timeout..!!");
        break;
       }
   val_2=digitalRead(sen2);
    }
    if( val_2 == HIGH){
        Serial.println("Person In..!!");
        total++;
      }
      if(prev!=total){
        Serial.println(" change occured ");
        Serial.print("Someone Entered In:");
        client.println(" change occured ");
        client.print("Someone Entered In:");
        displayTime();
      }
  }

else if((val_2==HIGH && val_1==LOW))
{
  currentMillis=millis();
  previousMillis=currentMillis;
  prev=0;
  prev=total;
  Serial.println("In out condition");
  //val_1=0;
  while(val_1 !=HIGH)
   {
    currentMillis=millis();
    if((currentMillis-previousMillis)>=interval)
     {
       Serial.println("timeout...!");
       break;
     }
  val_1=digitalRead(sen1);
   }
   
if(val_1==HIGH)
  {
   Serial.println("person out"); // if you put in if-else ststement then you can check only one sensors and if you put in only if condition then you shold check the status of all sensors connected.
       // out=1;
   total--;
   }
 if(prev!=total){
    Serial.println(" change occured ");
    Serial.print("Someone Leaved out:");
    client.println(" change occured ");
    client.print("Someone Leaved out:");
    displayTime();
  }
}
else if((val_2 == HIGH && val_1 == HIGH))
{
  Serial.println("Both are High");
  client.println("Both are High");
}

actual_strength();
}

void setup()
{
  pinMode(sen1,INPUT);
  pinMode(sen2,INPUT);
  Serial.begin(9600);
  Wire.begin();
  Ethernet.begin(mac, ip,gateway,subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
void loop()
{
  object_counter();
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
     currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
       // Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta http-equiv=\"refresh\" content=\"2\">");
          client.println("<title>Arduino Person Counter</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Hello from Arduino!</h1>");
          client.println("<p>A web page from the Arduino server with an detail of number of persons in a room</p>");
          displayTime();
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') 
        {
         // last character on line of received text
         // starting new line with next character read
          currentLineIsBlank = true;
         } 
        else if (c != '\r') 
        {
            // a text character was received from client
            currentLineIsBlank = false;
        }
      }
    }
    delay(15);
    // close the connection:
    client.stop();
   // Serial.println("client disconnected");
}
//object_counter();
}


 
