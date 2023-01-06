
#include <NewPing.h>
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define SONAR_NUM 3 //number of sensor
#define MaxDistance 600  //Maxium distance (in cm) to ping.

#define IN1 3 //left
#define IN2 2 
#define IN3 4 //right
#define IN4 7

#define ENL 5 //left
#define ENR 6 //right

int robot_width = 200; // 로봇의 너비 
int mazor_width = 500; // 미로의 폭
int mazor_width_tolerance = 10; // 센서의 오차범위
int measure_width; // 총 산출값





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


void Turning(int L_Ms, int R_Ms)
{
  if(L_sonar_Distance < 100)
  {
   Right(90,90);
  }
  else if(R_sonar_Distance < 200)
  {
   Left(90,90);
  }
  else
  Straight(70,80);
}

 
void moter(int L_Ms, int R_Ms)
{

      
   Straight(L_Ms, R_Ms);
        
         
   
  
}




void read_sonar_sensor(void) //초음파 센서 값을 읽는 함수
{
   R_sonar_Distance = sonar[0].ping_cm()*10; //각자에 맞도록 센서 번호 설정
   R_sonar_Distance = sonar[1].ping_cm()*10;
   L_sonar_Distance = sonar[2].ping_cm()*10;
   if( R_sonar_Distance == 0) R_sonar_Distance = MaxDistance*10.0;
   if( L_sonar_Distance == 0) L_sonar_Distance = MaxDistance*10.0;
   if( F_sonar_Distance == 0) F_sonar_Distance = MaxDistance*10.0;
}

/*
void updata_sonar_old(void)
{
 R_sonar_Distance_old = R_sonar_Distance; //시간이 지남에따라 현재값을 올드 값으로 넘겨줌
 L_sonar_Distance_old = L_sonar_Distance;
 F_sonar_Distance_old = F_sonar_Distance;
}
*/

void updata_sonar_error(void)
{
 R_sonar_Error = R_sonar_Distance - L_sonar_Distance; //
 L_sonar_Error = L_sonar_Distance - R_sonar_Distance;
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
  
  Serial.begin(9600);

  
}

void loop()
{
 float p_gain = 10;
 int moter_diff_R, moter_diff_L = 0;
 
 read_sonar_sensor(); // 센서 읽고
 updata_sonar_error(); //차이값을 구하고

 measure_width = L_sonar_Distance + robot_width + R_sonar_Distance;
 
 Serial.print("L_sonar : ");
 Serial.print(L_sonar_Distance); Serial.print("   "); //현재값 모니터 출력
 Serial.print("R_sonar : ");
 Serial.print(R_sonar_Distance); Serial.println(""); //현재값 모니터 출력
 
 Serial.print("L_sonar_Error : ");Serial.println(L_sonar_Error); //차이값 모니터 출력
 Serial.print("R_sonar_Error : ");Serial.println(R_sonar_Error); //차이값 모니터 출력

 moter_diff_R = R_sonar_Error * p_gain;// 각도 = 차이값*상수
 moter_diff_L = L_sonar_Error * p_gain;

if(measure_width*0.9 - mazor_width_tolerance < measure_width < measure_width*1.0 + mazor_width_tolerance)
{ 
  Straight(50,80);
  if((R_sonar_Distance < L_sonar_Distance)||(R_sonar_Distance < 200))
  {
    moter(30 - moter_diff_R, 180 + moter_diff_R); //
  }   
  else if((L_sonar_Distance < R_sonar_Distance)||(L_sonar_Distance < 200))
  {
    moter(100 + moter_diff_L, 100 - moter_diff_L);
  }
}
 delay(200);

 //이후 조정치 만큼 바퀴의 각도 수정
}
