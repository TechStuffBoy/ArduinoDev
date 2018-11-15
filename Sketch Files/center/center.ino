const char* text = "hello";
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  center(text,1);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void center(const char* text, byte textSize ){
  String words=text;
  char str[50];
  int len;
  strcpy(str, text);
  len = strlen(str);
  //Serial.println(str);
  //Serial.println(len);
  if(textSize == 1){
    int space= 26;
    int x=space / 2 + len / 2;
    for(int i=0; i<=x ;i++){
      Serial.print(" ");
    }
    Serial.println(str);
  }
  memset(str,0,50);
  Serial.println(str);
  
}


/*
 * int main ()
{
   char str[50];
   int len;

   strcpy(str, "This is tutorialspoint.com");

   len = strlen(str);
   printf("Length of |%s| is |%d|\n", str, len);
   
   return(0);
}
 */
