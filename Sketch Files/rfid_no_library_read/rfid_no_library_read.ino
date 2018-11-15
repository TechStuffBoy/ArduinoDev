String gettag="";
int bytesread=0,val=0;
void setup()
{
  Serial.begin(9600);
  pinMode(2,OUTPUT);   // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin
  digitalWrite(2,HIGH);
}
void loop()
{
   if(Serial.available()>0) // if data available from reader
    {         
        if((val=Serial.read())==10)  // check for header
        {
            bytesread=0;
            while(bytesread<10)  // read 10 digit code
            {
                if(Serial.available()>0)
                {
                    val=Serial.read();
                    if((val==10)||(val==13))  // if header or stop bytes before the 10 digit reading
                    {
                      break;  // stop reading
                    }
                    gettag=gettag+val;  // add the digit          
                    bytesread++;  // ready to read next digit 
                }
            }
            if(bytesread==10)  // if 10 digit read is complete
            {
                Serial.print("TAG code is: ");   // possibly a good TAG
                Serial.println(gettag);
            }
            bytesread=0;
            digitalWrite(2, LOW); // deactivate the RFID reader for a moment so it will not flood
            delay(1500);  // wait for a bit
            digitalWrite(2, HIGH);  // Activate the RFID reader
            gettag='\0';
        }
    }
}
