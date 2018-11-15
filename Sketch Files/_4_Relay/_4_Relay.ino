int relay1=3;
int relay2=4;
int relay3=5;
int relay4=6;
int r1;

void setup(){
  Serial.begin(9600);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
}
void loop(){
  
 digitalWrite( relay1, HIGH);
 Serial.print(relay1);
 delay(2000);
 digitalWrite( relay1, LOW);
 delay(2000);
  /*Serial.print("Relay1");
  Serial.println (relay1);
  delay(300);
  digitalWrite( relay1, LOW);
  delay(300);

//  digitalWrite( relay2, HIGH);
//  delay(300);
//  digitalWrite( relay2, LOW);
//  delay(300);
//
//  digitalWrite( relay3, HIGH);
//  delay(300);
//  digitalWrite( relay3, LOW);
//  delay(300);
//
//  digitalWrite( relay4, HIGH);
//  delay(300);
//  digitalWrite( relay4, LOW);
//  delay(300);
//  
*/
}

