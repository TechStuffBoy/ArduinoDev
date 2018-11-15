#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include <SPI.h>



byte mac[] = {
	0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
IPAddress ip(192, 168, 1, 40);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);

void setup()
{
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	Serial.println("Trying to get an IP address using DHCP");
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// initialize the ethernet device not using DHCP:
		Ethernet.begin(mac, ip, gateway, subnet);
	}
	// print your local IP address:
	Serial.print("My IP address: ");
	ip = Ethernet.localIP();
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(ip[thisByte], DEC);
		Serial.print(".");
	}
	Serial.println();
	// start listening for clients
	server.begin();

}
void loop()
{
	EthernetClient client=server.available();
	if(client)
	{
		Serial.println("We Got a New Client");
		boolean currentLineIsBlank = true;
		while(client.connected())
		{
			if(client.available())
			{
				char c=client.read();
				Serial.write(c);

				if(c=='\n' && currentLineIsBlank ) //Lastline recieved from Webbrowser(Client)
				{
					//Sending response back to Browser from arduino
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");
					
					client.println();
					//Send Webpage
					client.println("<!DOCTYPE html>");
					client.println("<html>");
					client.println("<head>");
					client.println("<meta http-equiv=\"refresh\" content=\"5\">");
					client.println("<title>Arduino Web Page</title>");
					client.println("</head>");
					client.println("<body>");
					client.println("<h1>Hello from Arduino!</h1>");
					client.println("<p>A web page from the Arduino server using DHCP with Analog Reading</p>");
					for (int analogChannel = 0; analogChannel < 6; analogChannel++)
					{
						int sensorReading = analogRead(analogChannel);
						client.print("analog input ");
						client.print(analogChannel);
						client.print(" is ");
						client.print(sensorReading);
						client.println("<br>");
					}
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
		delay(1);
		// close the connection:
		client.stop();
		Serial.println("client disconnected");
	}
}

