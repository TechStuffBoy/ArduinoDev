int Vib_sen=3;
int val=0;

void setup(){
  pinMode(Vib_sen, INPUT);
  Serial.begin(9600);
}
void loop(){

  val=digitalRead( Vib_sen );

  if(val){
    Serial.print( "Val :");
    Serial.println(val);
    Serial.println(" Vibration Occuring ");
}
delay(50);
}
