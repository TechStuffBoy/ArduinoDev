byte recd = 7;
boolean val = ((!recd == 0)? false:true);

void setup()
{
    Serial.begin(9600);
    Serial.println(String(val));
}
void loop(){
    
}


/*
 * label.frame = (inPseudoEditMode) ? kLabelIndentedRect : kLabelRect;
is semantically equivalent to

if(inPseudoEditMode) {
 label.frame = kLabelIndentedRect;
} else {
 label.frame = kLabelRect;
}
*/

