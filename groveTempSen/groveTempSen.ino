/*
/* Grove - Temperature Sensor demo v1.0
*  This sensor detects the environment temperature,
*  Connect the signal of this sensor to A0, use the 
*  Serial monitor to get the result.
*  By: http://www.seeedstudio.com
*/
#include <math.h>;
unsigned int a;
float temperature;
int B=3975;                  //B value of the thermistor
float resistance;
 
void setup()
{
  Serial.begin(9600);  
}
 
void loop()
{
  a=0;
  for(int i=0;i<20;i++){
      a +=  analogRead(0);
      delay(5);
  }
  Serial.print(a);
  a/=20;
  Serial.print("      ");
  Serial.println(a);
  resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
  temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet&nbsp;;
  delay(1000);
  Serial.print("Current temperature is ");
  Serial.println(temperature);
 }
