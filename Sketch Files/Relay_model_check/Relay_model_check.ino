int arduino_ip_1=2;
int arduino_ip_2=3;
int arduino_ip_3=4;
int arduino_ip_4=5;

int arduino_op_1=8;
int arduino_op_2=9;
int arduino_op_3=10;
int arduino_op_4=11;

void setup()
{
 pinMode(arduino_ip_1,INPUT);
 pinMode(arduino_ip_2,INPUT);
 pinMode(arduino_ip_3,INPUT);
 pinMode(arduino_ip_4,INPUT);

 pinMode(arduino_op_1,OUTPUT);
 pinMode(arduino_op_2,OUTPUT);
 pinMode(arduino_op_3,OUTPUT);
 pinMode(arduino_op_4,OUTPUT);
 Serial.begin(9600);
 
 }

 void loop(){
//  Serial.print(digitalRead(arduino_ip_1));
//  Serial.print(",");
//  Serial.print(digitalRead(arduino_ip_2));
//  Serial.print(",");
//  Serial.print(digitalRead(arduino_ip_3));
//  Serial.print(",");
//  Serial.println(digitalRead(arduino_ip_4));
//  delay(1000);
  if(digitalRead(arduino_ip_1)==HIGH){
    Serial.println(digitalRead(arduino_ip_1));
    digitalWrite(arduino_op_1,HIGH);
    delay(1000);
    digitalWrite(arduino_op_1,LOW);
    delay(1000);
    }

    if(digitalRead(arduino_ip_2)==HIGH){
    digitalWrite(arduino_op_2,HIGH);
    delay(1000);
    digitalWrite(arduino_op_2,LOW);
    delay(1000);
    }

    if(digitalRead(arduino_ip_3)==HIGH){
    digitalWrite(arduino_op_3,HIGH);
    delay(1000);
    digitalWrite(arduino_op_3,LOW);
    delay(1000);
    }

    if(digitalRead(arduino_ip_4)==HIGH){
    digitalWrite(arduino_op_4,HIGH);
    delay(1000);
    digitalWrite(arduino_op_4,LOW);
    delay(1000);
    }
 }

