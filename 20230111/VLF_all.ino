
#include <NewPing.h>
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define SONAR_NUM 3 //number of sensor
#define MaxDistance 600  //Maxium distance (in cm) to ping.
#define Obstancle 20

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


int A;
int L_Ms, R_Ms;
int line_sensor[5] = {0,0,0,0,0};


float R_sonar_Distance = 0.0; //현재 소나센서 측정값
float R_sonar_Distance_old = 0.0; //전에 측정했던 과거값
float R_sonar_Error = 0.0; //현재 - old

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


void obstacle_detection()
{
  while(1)
  {
  if(F_sonar_Distance <= 10)
    motor_control(0,0,0,0);
  else
   {
    line_following();
    break;
   }  
  } 
}

void motor_control(int dir_l, int L_Ms, int dir_r, int R_Ms)
{
 if(dir_l == 1) //전진
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENL, L_Ms);
 }
 else if(dir_l == -1)//후진
  {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENL, L_Ms);
  } 
 else
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENL, 0);
 }

 if(dir_r == 1)
 {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENR, R_Ms);
 }
 else if(dir_r == -1)
 {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENR, R_Ms);
 }
 else
 {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENR, 0);
 }

}  
  
int line_following()
{
  int i,sum = 0; //초기화
  int line_index;
  
  for(i=0; i<5; i++)
  {
    line_sensor[i] = digitalRead(A0+i);//아날로그 핀(A0) [흰바탕에 검은색 인식] , 라인 트레이싱 시에는 (1-) 삭제, sum =을 5 로 바꿈
    sum += line_sensor[i];
    Serial.print(line_sensor[i]);
    Serial.print("  ");
  }
    Serial.println("");

  //line sensor[0] line sensor[1] line sensor[2] line sensor[3] line sensor[4] 
  //   -4      -3      -2      -1       0       1       2       3       4

if(sum == 5) line_index = -10; 

if(sum == 1)
{
  if(line_sensor[0]==1) line_index = -4;
  if(line_sensor[1]==1) line_index = -2;
  if(line_sensor[2]==1) line_index =  0;
  if(line_sensor[3]==1) line_index =  2;
  if(line_sensor[4]==1) line_index =  4;
}

if(sum == 2)
{
  if((line_sensor[0]==1) && (line_sensor[1]==1) ) line_index = -3;
  if((line_sensor[1]==1) && (line_sensor[2]==1) ) line_index = -1;
  if((line_sensor[2]==1) && (line_sensor[3]==1) ) line_index =  1;
  if((line_sensor[3]==1) && (line_sensor[4]==1) ) line_index =  3;
}

Serial.print(line_index);
Serial.println("");

  return line_index;
 
}


 

void cwrotation()//회전
{
  
  double min_dg = 0.0; //각도 측정 최솟값
  double min_dg2 = 0.0; 
  min_dg = F_sonar_Distance;
  for(int i=0; i<400; i++)
  {   
    motor_control(1,90,1,90);
        
    min_dg2 = F_sonar_Distance;
    if (min_dg > min_dg2)
    {
      min_dg = min_dg2;
    }
    
    if(F_sonar_Distance < MaxDistance)
    {
      F_sonar_Distance = MaxDistance;
    }
    Serial.println(F_sonar_Distance);
    
  }
   motor_control(1,0,1,0);
   delay(1000);
   Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
   Serial.println(min_dg);    //최소값 추출 완료
   Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
   
   while(1)
   {     motor_control(1,90,-1,90);
    
  
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
            Serial.println(L_sonar_Distance);     //최소값을 따라가야할 L값
            Serial.println(R_sonar_Distance);     //최소값을 따라가야할 R값
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
         if(L_sonar_Distance < min_dg + 1 && L_sonar_Distance > min_dg - 1)
         {
          delay(250);
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"); 
            Serial.println("왼쪽이 더먼저 최소값에 도달하였습니다. 정지합니다.");     //정상작동할시 뜨는 문구
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
         motor_control(1,0,-1,0);
         break;
         
         }
         else if(R_sonar_Distance < min_dg + 1 && R_sonar_Distance > min_dg - 1)
         {
          delay(250);
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"); 
            Serial.println("오른쪽이 더먼저 최소값에 도달하였습니다. 정지합니다.");     //정상작동할시 뜨는 문구
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
          motor_control(1,0,-1,0);
          break;
         }
  }
}
  
void left_wall_following(int moter_diff_L)//
{
  Serial.print("L_sonar : ");
  Serial.print(L_sonar_Distance); Serial.print("   "); //현재값 모니터 출력
  
  Serial.print("L_sonar_Error : ");Serial.println(L_sonar_Error); //차이값 모니터 출력
  
  if(L_sonar_Distance < R_sonar_Distance)
  {
     motor_control(1, 140 + moter_diff_L, 1, 140 - moter_diff_L);
  }
  
}


void moter_tracking(int moter_diff_L, int moter_diff_R)//양쪽벽 트레킹
{
  Serial.print("L_sonar : ");
  Serial.print(L_sonar_Distance); Serial.print("   "); //현재값 모니터 출력
  Serial.print("R_sonar : ");
  Serial.print(R_sonar_Distance); Serial.println(""); //현재값 모니터 출력
 
  Serial.print("L_sonar_Error : ");Serial.println(L_sonar_Error); //차이값 모니터 출력
  Serial.print("R_sonar_Error : ");Serial.println(R_sonar_Error); //차이값 모니터 출력

  if((R_sonar_Distance < L_sonar_Distance)&&(R_sonar_Distance < 200))
  {
    motor_control(1, 30 - moter_diff_R, 1, 180 + moter_diff_R); //오른족으로
  }   
  else if((L_sonar_Distance < R_sonar_Distance)&&(L_sonar_Distance < 200))
  {
    motor_control(1, 140 + moter_diff_L, 1, 100 - moter_diff_L);
  }
}


void read_sonar_sensor(void) //초음파 센서 값을 읽는 함수
{
   F_sonar_Distance = sonar[0].ping_cm()*10; //각자에 맞도록 센서 번호 설정
   R_sonar_Distance = sonar[1].ping_cm()*10;
   L_sonar_Distance = sonar[2].ping_cm()*10;
   if( R_sonar_Distance == 0) R_sonar_Distance = MaxDistance*10.0;
   if( L_sonar_Distance == 0) L_sonar_Distance = MaxDistance*10.0;
   if( F_sonar_Distance == 0) F_sonar_Distance = MaxDistance*10.0;
}


void updata_sonar_old(void)
{
 R_sonar_Distance_old = R_sonar_Distance; //시간이 지남에따라 현재값을 올드 값으로 넘겨줌
 L_sonar_Distance_old = L_sonar_Distance;
 F_sonar_Distance_old = F_sonar_Distance;
}


void updata_sonar_error(void)
{
 R_sonar_Error = R_sonar_Distance - L_sonar_Distance_old; //현재값-old값
 L_sonar_Error = L_sonar_Distance - R_sonar_Distance_old;
 F_sonar_Error = F_sonar_Distance - F_sonar_Distance_old;
}

int mission_flag(int i)
{
  if(i==1) return 1;
  else if(i==2)return 2;
  else return 0;
}

void wall_detection()
{
  
  if((measure_width*0.9 - mazor_width_tolerance < measure_width < measure_width*1.0 + mazor_width_tolerance)&&(line_following == 0))
  {
    motor_control(0,0,0,0);
    mission_flag(1);
  }
  
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
  
  int index = line_following();
  
  while(index != -10){
    line_following();
    switch(index){
    
    case  -10 ://정지
           motor_control(1,0,1,0);
           Serial.print("정지");
           return -10;
           break;
           
    case  0 : //전진
           motor_control(1,60,1,60);
           Serial.println("전진");
           break;

    case 1 : //좌회전
            motor_control(1,70,1,100);
            Serial.println("좌회전");
            break;

    case 3 : //급 좌회전
            motor_control(1,50,1,110);
            Serial.println("급 좌회전");
            break;

    case 4 : //직각 좌회전
            motor_control(1,30,1,120);
            Serial.println("직각 좌회전");
            break;

    case -1: //우회전
            motor_control(1,100,1,70);
            Serial.println("우회전");
            break;

    case -3 : //급 우회전
            motor_control(1,110,1,50);
            Serial.println("급 우회전");
            break;
            
    case -4 : //직각 우회전
            motor_control(1,30,1,120);
            Serial.println("직각 우회전");
            break;        
            }
      }
       


  float p_gain = 3;
  int motor_diff_R, motor_diff_L = 0;
 
  read_sonar_sensor(); // 센서 읽고
  updata_sonar_error(); //차이값을 구하고
  updata_sonar_old();//현재값->old값

  measure_width = L_sonar_Distance + robot_width + R_sonar_Distance; //총 산출값 = 

  motor_diff_R = R_sonar_Error * p_gain;// 각도 = 차이값*상수
  motor_diff_L = L_sonar_Error * p_gain;

  


  if(mission_flag == 0)
  {
    line_following();
    obstacle_detection();//초음파 센서가 장애물이 있으면 멈추고 없으면 이동
    wall_detection();//양쪽 벽을 인식-> 잠깐 정지  mission_flag = 1
  }
  if(mission_flag == 1)
  {
    left_wall_following(motor_diff_L);//회전조건 조사 -> mission_flag ==2
  }
  if(mission_flag == 2);
  {
    cwrotation();
  }
  
}
