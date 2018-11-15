#include<SPI.h>
#include<Ethernet.h>
boolean currentLineIsBlank=true;
int sen1=5;
int sen2=6;
int val1=0;
int val2=0;
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress ip(192, 168, 1, 17);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);
EthernetClient client;
void setup()
{
  pinMode(sen1,INPUT);
  pinMode(sen2,INPUT);
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip,gateway,subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
     currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
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
          client.println("Hello");
          client.println(con());
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
    delay(2);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
}

}
void con()
{
  if(val1>0 && val2<0)
          return "val1 is high";
       else if(val2>0 && val1<0)
          return "val1 is high";  
          else if(val1>0 && val2>0)
            return"both are high";
            else 
              return "no condition"
}

