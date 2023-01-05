
#include <NewPing.h>
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define SONAR_NUM 3 //number of sensor
#define MaxDistance 150  //Maxium distance (in cm) to ping.

#define IN1 3 //left
#define IN2 2 
#define IN3 4 //right
#define IN4 7

#define ENL 5 //left
#define ENR 6 //right


int a;
int L_Ms, R_Ms;

float R_sonar_Distance = 0.0; //현재 소나센서 측정값
float R_sonar_Distance_old = 0.0; //전에 측정했던 과거값
float R_sonar_Error = 0.0; //둘의 차이값

float L_sonar_Distance = 0.0;
float L_sonar_Distance_old = 0.0;
float L_sonar_Error = 0.0;

float F_sonar_Distance = 0.0;
float F_sonar_Distance_old = 0.0;
float F_sonar_Error = 0.0;



NewPing sonar[SONAR_NUM]= {   //Sensor object array.
NewPing (8,8,MaxDistance), //F   Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing (10,10,MaxDistance), //R
NewPing (11,11,MaxDistance) //L
};


void Straight(int L_Ms, int R_Ms)
{
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 analogWrite(ENL, L_Ms);
 analogWrite(ENR, R_Ms);
 delay(a);
}

void Back(int L_Ms, int R_Ms)
{
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);
 analogWrite(ENL, L_Ms);
 analogWrite(ENR, R_Ms);
 delay(300);
}

void Right(int L_Ms, int R_Ms)
{
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH); 
 digitalWrite(IN4, LOW);
 analogWrite(ENL, L_Ms);
 analogWrite(ENR, R_Ms);
 delay(a);
}

void Left(int L_Ms, int R_Ms)
{
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 analogWrite(ENL, L_Ms);
 analogWrite(ENR, R_Ms);
 delay(a);
}

void Stop()
{
  analogWrite(ENL,0);
  analogWrite(ENR,0);
  delay(1000);
}

/*
void Turning(int a)
{
  if(L_Sonar_sensor() < 100)
  {
   Right(a);
  }
  else if(R_Sonar_sensor() < 200)
  {
   Left(a);
  }
  else
  Straight(a);
*/

 
void moter(int L_Ms, int R_Ms)
{
  Straight(L_Ms, R_Ms);
   
  
}




void read_sonar_sensor(void) //초음파 센서 값을 읽는 함수
{
   R_sonar_Distance = sonar[1].ping_cm()*10; //각자에 맞도록 센서 번호 설정
   L_sonar_Distance = sonar[2].ping_cm()*10;
   F_sonar_Distance = sonar[0].ping_cm()*10;
   if( R_sonar_Distance == 0) R_sonar_Distance=MaxDistance*10.0;
   if( L_sonar_Distance == 0) L_sonar_Distance=MaxDistance*10.0;
   if( F_sonar_Distance == 0) F_sonar_Distance=MaxDistance*10.0;
}


void updata_sonar_old(void)
{
 R_sonar_Distance_old = R_sonar_Distance; //시간이 지남에따라 현재값을 올드 값으로 넘겨줌
 L_sonar_Distance_old = L_sonar_Distance;
 F_sonar_Distance_old = F_sonar_Distance;
}


void updata_sonar_error(void)
{
 R_sonar_Error = R_sonar_Distance - R_sonar_Distance_old; //현재값-과거값=에러값
 L_sonar_Error = L_sonar_Distance - L_sonar_Distance_old;
 F_sonar_Error = F_sonar_Distance - F_sonar_Distance_old;
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
  
  Serial.begin(115200);

  
  delay(1000);

}

void loop()
{
 float p_gain = 1.5;
 int moter_diff = 0;
 
 read_sonar_sensor(); // 센서 읽고
 updata_sonar_error(); //차이값을 구하고
 updata_sonar_old(); // 현재값을 과거값으로 넘겨줌
 
 Serial.print(R_sonar_Distance); Serial.print(" "); //현재값 모니터 출력
 Serial.print(R_sonar_Distance_old); Serial.println("   "); //과거값 모니터 출력
 Serial.println(R_sonar_Error); //차이값 모니터 출력

 moter_diff = R_sonar_Error * p_gain; // 각도 = 차이값*1.5
 moter(100 - moter_diff, 140 + moter_diff); // 
 delay(250);

 //이후 조정치 만큼 바퀴의 각도 수정
}
