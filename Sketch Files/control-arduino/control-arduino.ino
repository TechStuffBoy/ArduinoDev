
const int conveyor=2;
const int crank=3;
const int sensor1=4;
const int sensor1Val=0;
const int sensor1Count=0;


void setup() {
  pinMode (conveyor, OUTPUT);
  pinMode(crank, OUTPUT);
  pinMode (sensor1, INPUT);
  Serial.begin(9600);
  
  
}

void loop()
{
 // digitalWrite(conveyor, HIGH);
  //Serial.println ("Conveyor on");
 // delay(2000);
  digitalWrite(crank, HIGH);
  Serial.println ("Crank on");
 /* sensor1Val = digitalRead(sensor1);
  while (sensor1Val!=0)
  {
     
     while(!sensor1Val)
      {
       sensor1Val = digitalRead(sensor1);
       sensor1Count++;
       Serial.print ("incoming :");
       Serial.println(sensor1Count);
       break;
     }
  }
  
 */ 
}

