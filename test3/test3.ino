void setup()
{

  /* add setup code here */
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  

}

void loop()
{
	Serial.println("Nandhu");
	digitalWrite(13, HIGH);
	delay(1000);
	digitalWrite(13,LOW);
	delay(1000);
	  /* add main program code here */

}
