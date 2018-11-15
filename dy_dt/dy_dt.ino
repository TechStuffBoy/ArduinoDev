
float  prevX = 0;
float curX = 0;
float  dx = 0;
float dt = 0;
float dx_dt = 0;

float  prevY = 0;
float curY = 0;
float  dy = 0;
float dy_dt = 0;

float  prevZ = 0;
float curZ = 0;
float  dz = 0;
float dz_dt = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    prevX = analogRead(0);
    prevY = analogRead(1);
    prevZ = analogRead(2);
}

void loop() {
    prevX = curX;
    prevY = curY;
    prevZ = curZ;
    for(int i =0; i<25 ; i++){
    curX += analogRead(0);
    curY += analogRead(1);
    curZ += analogRead(2);
    delay(2);
    }
    curX = curX/25.0;
    curY = curY/25.0;
    curZ = curZ/25.0;
    
    dx = (curX - prevX);
    dt =  1.0 - 0.050;
    dx_dt = dx/dt;

    dy = (curY - prevY);
    dt =  1.0 - 0.050;
    dy_dt = dy/dt;

    dz = (curZ - prevZ);
    dt =  1.0 - 0.050;
    dz_dt = dz/dt;
    
//    Serial.println(dz_dt);

    Serial.print("dx_dt :");
    Serial.print(dx_dt);
    Serial.print("\tdy_dt :");
    Serial.print(dy_dt);
    Serial.print("\tdz_dt :");
    Serial.println(dz_dt);
}
