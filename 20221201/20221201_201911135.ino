#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define Obstancle 20

#define IN1 3 
#define IN2 2 
#define IN3 4
#define IN4 7

#define ENA 5
#define ENB 6

int robot_width = 200; // 로봇의 너비 
int mazor_width = 500; // 미로의 폭
int mazor_width_tolerance = 40; // 센서의 오차범위
int measure_width; // 총 산출값

int a;

float F_Sonar_sensor()
{
  float duration = 0.0;
  float distance = 0.0;
  pinMode(F_Sonar,OUTPUT);
  digitalWrite(F_Sonar,LOW);
  digitalWrite(F_Sonar,HIGH);
  delay(10);
  digitalWrite(F_Sonar,LOW);
  pinMode(F_Sonar,INPUT);
  duration = pulseIn(F_Sonar,HIGH);
  distance =(duration * 340.0)/2.0/1000.0;
  return distance;
}

float R_Sonar_sensor()
{
  float duration = 0.0;
  float distance = 0.0;
  pinMode(R_Sonar,OUTPUT);
  digitalWrite(R_Sonar,LOW);
  digitalWrite(R_Sonar,HIGH);
  delay(10);
  digitalWrite(R_Sonar,LOW);
  pinMode(R_Sonar,INPUT);
  duration = pulseIn(R_Sonar,HIGH);
  distance =(duration * 340.0)/2.0/1000.0;
  return distance;
}

float L_Sonar_sensor()
{
  float duration = 0.0;
  float distance = 0.0;
  pinMode(L_Sonar,OUTPUT);
  digitalWrite(L_Sonar,LOW);
  digitalWrite(L_Sonar,HIGH);
  delay(10);
  digitalWrite(L_Sonar,LOW);
  pinMode(L_Sonar,INPUT);
  duration = pulseIn(L_Sonar,HIGH);
  distance =(duration * 340.0)/2.0/1000.0;
  return distance;
  
}

void Straight(int a)
{
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 analogWrite(ENA, 35);
 analogWrite(ENB, 50);
 delay(a);
}

void Back(int a)
{
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);
 analogWrite(ENA, 80);
 analogWrite(ENB, 80);
 delay(300);
}

void Right(int a)
{
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH); 
 digitalWrite(IN4, LOW);
 analogWrite(ENA, 90);
 analogWrite(ENB, 100);
 delay(a);
}

void Left(int a)
{
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 analogWrite(ENA, 90);
 analogWrite(ENB, 110);
 delay(a);
}

void Stop(int a)
{
  analogWrite(ENA,0);
  analogWrite(ENB,0);
  delay(1000);
}

void Turning(int a)
{
  if(L_Sonar_sensor() < 200)
  {
   Right(a);
  }
  else if(R_Sonar_sensor() < 200)
  {
   Left(a);
  }
  else
  Straight(a);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(F_Sonar, OUTPUT);
  pinMode(R_Sonar, OUTPUT);
  pinMode(L_Sonar, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  delay(3000);
}



void loop()
{
  float L_distance, F_distance, R_distance;
  L_distance = L_Sonar_sensor();
  F_distance = F_Sonar_sensor();
  R_distance = R_Sonar_sensor();
  Serial.print("L:"); Serial.print(L_distance); 
  Serial.print("  F:"); Serial.print(F_distance); 
  Serial.print("  R:"); Serial.println(R_distance); 
  
  measure_width = L_distance + robot_width + R_distance; 

  if(F_distance < 150)
  {
    Stop(a);
    if((L_distance < 200)||(R_distance < 200))
    {
      Turning(700);
      Stop(a);
    }
  }  
  else if((L_distance < 60)||(R_distance < 60))
  { 
     Turning(1);
  }
  else
  {
    Straight(a);
  }
}
