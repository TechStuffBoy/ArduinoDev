String one = "hello";
String two = "hello";

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  if( one == two ){
    Serial.println("Equal");
  } else {
      Serial.println("Not Equal");
    }
}

