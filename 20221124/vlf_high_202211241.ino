// C++ code
//
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define Obstancle 20

int flag_mazor_1 = 0; // 1이면 1번구간에 내가 위치해 있다

int robot_width = 200; // 로봇의 너비 
int mazor_width = 500; // 미로의 폭
int mazor_width_tolerance = 40; // 센서의 오차범위
int measure_width; // 총 산출값


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
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(F_Sonar, OUTPUT);
  pinMode(R_Sonar, OUTPUT);
  pinMode(L_Sonar, OUTPUT);
  
  Serial.begin(115200);
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
  
  measure_width = L_distance + robot_width + R_distance; // 총 산출값 = 원쪽 센서 감지거리 + 오른쪽 센서 감지거리 + 로봇의 너비
  
  if((measure_width >= (mazor_width - mazor_width_tolerance))&&(measure_width<=(mazor_width + mazor_width_tolerance))) // 만약 (총 산출값 >= 460), (총 산출값 <=540)가 모두 참일 경우 
  {
    	if ((F_distance > mazor_width)&&(F_distance < mazor_width + 400)) // (전방 센서 감지값 > 500), (전방 센서 감지값 < 미로의 폭(500) + 400) 두 조건이 모두 참 이라면
    {
          flag_mazor_1 = 1;  // 미로의 falg 값이 1 이다 = 현재 로봇이 1에 위치해 있음
          Serial.println("flag_mazor_1 set"); // 화면에 flag_mazor_1 set을 출력
    }
  }
    else // 혹 거짓 이라면
  {
      flag_mazor_1 = 0; // falg 값은 0이다 = 로봇의 현 위치값은 1이 아님
  }
    
  
}
