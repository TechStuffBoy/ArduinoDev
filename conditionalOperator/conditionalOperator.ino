boolean val = true;
String compLHSGlobal = "DI_00";
String hi = "";
String one;
#define DI_00 25
void setup()
{
	char buff[10];
	Serial.begin(9600);
	one = "hello";
	one += "World";
	Serial.print("one :");
	Serial.println(one);
	(val == true)? "true":"false";
	Serial.print("compLHSGlobal :");
	Serial.println((compLHSGlobal.equals("DI_008")));
	Serial.println(String((val == true)? "true":"false"));
	one = "hi";
	Serial.println(one);
	if( (val == true)? "true":"false" == "true"){
		Serial.println("You Got :");
	}
	
	Serial.println(hi.toInt());
	Serial.println(compLHSGlobal.toInt());
}

void loop()
{
	int commaIndex = compLHSGlobal.indexOf('_');
	String firstValue = compLHSGlobal.substring(commaIndex+1);
	Serial.println(firstValue);
	int whichPortNo = firstValue.toInt();
	Serial.println(whichPortNo);
	while(1);
}

