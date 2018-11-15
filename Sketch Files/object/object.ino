//int sen_p1=0;
int sen_p2=1;
//int sense1=0;
int sense2=0;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  sense2=analogRead(sen_p2);
 // sense2=analogRead(sen_p2);
  Serial.print(sense2);
  Serial.println();
//  Serial.print(sense2);
//  Serial.println();
  Serial.print("_ _ _ _ _ _ _ _ _ _ _ _ _ _ ");
  Serial.println();
  delay(1000);
}

