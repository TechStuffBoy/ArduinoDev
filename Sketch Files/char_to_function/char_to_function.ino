/*
void setup(){
  Serial.begin(9600);
}

void loop(){
  char *str = myFunction();
  Serial.println(str); // prints "returnedText"
}
char *myFunction()
{
 return "returnedText"; // this returns a pointer to the string, not the string itself
}

*/
/*
#define MAX_LEN 5
char arr[ MAX_LEN ] = { 1, 2, 3, 4, 5 };
 
void Func( char *ptr ){
   
  for( int idx = MAX_LEN - 1 ; idx >= 0 ; --idx ){
    Serial.println( ptr[ idx ], HEX );
  }
}
 
void setup(){
  Serial.begin( 9600 );
  Func( arr );
}
 
void loop() {}
*/
char arr[56];
void Func( char (&array)[56] ){
 
  for( int idx = 0 ; idx < sizeof( array ) ; ++idx ){
    Serial.print( array[ idx ]);
  }
}
 
void setup() {
  Serial.begin( 9600 );
   
char  arr[66] = "hello";
  Func(&arr);
  memset(arr,0,56);
  char arr[66]="machi";
  Func(&arr);
}
 
void loop(){}
