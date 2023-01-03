#include <NewPing.h>
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11
#define MaxDistance  500  //

#define IN1 3 //left
#define IN2 2 
#define IN3 4 //right
#define IN4 7

#define ENL 5 //left
#define ENR 6 //right

int a;
float L_DA1, L_DA2;
float R_DA1, R_DA2;

NewPing F_Sensor(F_Sonar,F_Sonar,MaxDistance);
float F_distance; 
float F_distance_D,F_distance1,F_distance2;

NewPing L_Sensor(L_Sonar,L_Sonar,MaxDistance);
float L_distance;
float L_distance_D,L_distance1,L_distance2;

NewPing R_Sensor(R_Sonar,R_Sonar,MaxDistance);
float R_distance;
float R_distance_D,R_distance1,R_distance2;



void Go_Chack(int a,int L_sp,int R_sp)
{
  //초기 센서 측정값
  F_distance1 = (float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  L_distance1 = (float)(L_Sensor.ping())*10/US_ROUNDTRIP_CM;
  R_distance1 = (float)(R_Sensor.ping())*10/US_ROUNDTRIP_CM;
  Go(a, L_sp, R_sp);
  
  F_distance2 = (float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM; //Go 움직인 후 측정값
  F_distance_D = F_distance1 - F_distance2; //거리의 차
  
  R_distance2 = (float)(R_Sensor.ping())*10/US_ROUNDTRIP_CM;
  R_distance_D = R_distance1 - R_distance2;
  
  L_distance2 = (float)(L_Sensor.ping())*10/US_ROUNDTRIP_CM;
  L_distance_D = L_distance1 - L_distance2;


  L_DA2 = L_distance_D/F_distance_D;
  R_DA2 = R_distance_D/F_distance_D;
  
  L_DA1 = atan(L_DA2)*180/3.14;
  R_DA1 = atan(R_DA2)*180/3.14;
}

void Left_chack(int a,int sp)
{
  F_distance1=(float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  L_distance1=(float)(L_Sensor.ping())*10/US_ROUNDTRIP_CM;
  R_distance1=(float)(R_Sensor.ping())*10/US_ROUNDTRIP_CM;
  Left_chack(a,sp);
  
  F_distance2=(float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  F_distance_D = F_distance1 - F_distance2;
  
  R_distance2=(float)(R_Sensor.ping())*10/US_ROUNDTRIP_CM;
  R_distance_D = R_distance1 - R_distance2;
  
  L_distance2=(float)(L_Sensor.ping())*10/US_ROUNDTRIP_CM;
  L_distance_D = L_distance1 - L_distance2;
  
  L_DA2 = L_distance_D/F_distance_D;
  R_DA2 = R_distance_D/F_distance_D;
  L_DA1 = atan(L_DA2)*180/3.14;
  R_DA1 = atan(R_DA2)*180/3.14;
}

void Right_chack(int a,int sp)
{
  F_distance1=(float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  L_distance1=(float)(L_Sensor.ping())*10/US_ROUNDTRIP_CM;
  R_distance1=(float)(R_Sensor.ping())*10/US_ROUNDTRIP_CM;
  Right_chack(a,sp);
  
  F_distance2 = (float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  F_distance_D = F_distance1 - F_distance2;
  
  R_distance2 = (float)(R_Sensor.ping())*10/US_ROUNDTRIP_CM;
  R_distance_D = R_distance1 - R_distance2;
  
  L_distance2 = (float)(L_Sensor.ping())*10/US_ROUNDTRIP_CM;
  L_distance_D = L_distance1 - L_distance2;
  
  L_DA2 = L_distance_D/F_distance_D;
  R_DA2 = R_distance_D/F_distance_D;
  L_DA1 = atan(L_DA2)*180/3.14;
  R_DA1 = atan(R_DA2)*180/3.14;
}



void Go(int a,int L_sp,int R_sp)
{
  analogWrite(ENL,L_sp); 
  analogWrite(ENR,R_sp);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  delay (a);
}

void Right(int a)
{
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH); 
 digitalWrite(IN4, LOW);
 analogWrite(ENL, 90);
 analogWrite(ENR, 100);
 delay(a);
}

void Left(int a)
{
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 analogWrite(ENL, 90);
 analogWrite(ENR, 110);
 delay(a);
}

void Stop(int a)
{
  analogWrite(ENL,0);
  analogWrite(ENR,0);
  delay(a);
}


void setup()
{
  Serial.begin(115200);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  analogWrite(ENL,80);
  analogWrite(ENR,80);
  
  Stop(100);
  digitalWrite(F_Sonar,LOW);
  digitalWrite(F_Sonar,HIGH);
  delay(1000);
  
  F_distance = (float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  
  Go_Chack(a,80,80);
  
  Serial.print("F_DA1 : ");
  Serial.println(F_distance1);
  digitalWrite(F_Sonar,LOW);

  Go_Chack(2000,80,80);
  
  delay(1000);
  Stop(a);

  digitalWrite(F_Sonar,HIGH);
  F_distance = (float)(F_Sensor.ping())*10/US_ROUNDTRIP_CM;
  Serial.print("F_DA2 : ");
  Serial.println(F_distance2);
  digitalWrite(F_Sonar,LOW);
  
  Serial.print("F_D : ");
  Serial.println(F_distance_D);
  Stop(100);
}

void loop()
{
  
}
