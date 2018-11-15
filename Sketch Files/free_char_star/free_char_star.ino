#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH  20

void freeStr(char **str)
{
    //free( *str );
    *str = NULL;   
}
void setup(){
  Serial.begin(9600);
}
void loop(){

    char *str = malloc( sizeof(char) * LENGTH);

    // clear that allocated memory
    memset( str, 0x00, LENGTH );

    // Put some text into the memory
    strncpy( str, "Some text", LENGTH-1 );

    Sserial.println("Before:" + str);


    freeStr(&str);
    // string is now NULL and the memory is free.
    Serial.println("After:"+ str);

    return;
}
