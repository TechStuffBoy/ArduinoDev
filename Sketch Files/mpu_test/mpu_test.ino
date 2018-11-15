#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

int En_1=10;
int En_2=11;
int Motor1_pin1=3;
int Motor1_pin2=4;
int Motor2_pin1=5;
int Motor2_pin2=6;

 
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
int valMotor1;
int valMotor2;
void setup()
{

pinMode( Motor1_pin1 ,OUTPUT);
pinMode( Motor1_pin2 ,OUTPUT);
pinMode( Motor2_pin1 ,OUTPUT);
pinMode( Motor2_pin2 ,OUTPUT);
pinMode( En_1 ,OUTPUT);
pinMode( En_2 ,OUTPUT);

Wire.begin();
Serial.begin(9600);
Serial.println("Initialize MPU");
mpu.initialize();
Serial.println(mpu.testConnection() ? "Connected" : "Connection failed"); 
}
void loop()
{
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 
ax = map(ax, -17000, 17000, -125, 125);
 
valMotor1 = 125+ax;
valMotor2 = 125-ax;

Serial.print("valMotor1 :");
Serial.println(valMotor1, DEC);
Serial.print("valMotor2 :");
Serial.println(valMotor2, DEC);
 
if(valMotor1 > 0 && valMotor1 < 255){
//motor1.setSpeed(valMotor1);
analogWrite(En_1, valMotor1);
}
if(valMotor2 > 0 && valMotor2 < 255){
//motor2.setSpeed(valMotor2);
analogWrite(En_2, valMotor2);
}
Motor_1_Fwd(); 
Motor_2_Bwd();
//motor1.run(FORWARD); // turn it on going forward
//motor2.run(BACKWARD);
 
delay(200);
 
}

void Motor_1_Fwd(){
  digitalWrite(Motor1_pin1, LOW );
  digitalWrite(Motor1_pin2, HIGH );
  
}

void Motor_2_Bwd(){
  digitalWrite(Motor2_pin1, LOW );
  digitalWrite(Motor2_pin2, HIGH );
  
}

