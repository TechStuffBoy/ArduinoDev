String Hello;
boolean flag = false;
boolean character = false;
const char* con;
void setup(){
  Serial.begin(9600);
  flag = true;
  character = true;
}

void loop(){
  
  if(flag == true){
    
      Hello = "How  are \0 you ";
      Serial.print("FirstString :");
      Serial.println(Hello);
      Hello= "I am ";
      Serial.print("SecondString :");
      Serial.println(Hello); 
      flag = false;
}

if( character == true){
      con = "I am character";
      Serial.print("FirstChar :");
      Serial.println(con);
      con="I am another Character";
      Serial.print("SecondChar :");
      Serial.println(con);
      character = false;
}
}
