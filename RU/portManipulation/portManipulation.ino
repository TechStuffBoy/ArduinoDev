byte my_var;
void setup() 
{
    Serial.begin(9600);
   
    DDRC = 0b00000000; //30,31,32,33,34,35,36,37
}

void loop()
{
         //Pin 1 of PORTD is an input, all others are outputs
         //Create a variable to store the data read from PORTD
 my_var = PINC ;   //Read the PORTD pin 1 value and put it in the variable
 Serial.println(String(my_var));
 delay(1000);
}

