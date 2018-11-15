
int ruid = 2;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  String msgInfo;
  char msgInfoBuff[40];
  msgInfo += ruid;
  msgInfo += "/OP_AUTH_RESP";
  msgInfo.toCharArray(msgInfoBuff,40);
  Serial.println(msgInfoBuff);
  while(1)
  ;
}

