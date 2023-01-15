
#include <NewPing.h>
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define SONAR_NUM 3 
#define MaxDistance 500

#define IN1 2 //left
#define IN2 3 
#define IN3 4 //right
#define IN4 7

#define ENL 5 //left
#define ENR 6 //right

int robot_width = 150; // 로봇의 너비 
int mazor_width = 1000; // 미로의 폭
int mazor_width_tolerance = 10; // 센서의 오차범위


int mission_flag = -1;
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

float p_gain = 3;



int measure_width = L_sonar_Distance + robot_width + R_sonar_Distance;


NewPing sonar[SONAR_NUM]= {   //Sensor object array.
NewPing (8,8,MaxDistance), //F   Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing (10,10,MaxDistance), //R
NewPing (11,11,MaxDistance) //L
};




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
 R_sonar_Error = R_sonar_Distance - R_sonar_Distance_old; //현재값-old값
 L_sonar_Error = L_sonar_Distance - L_sonar_Distance_old;
 F_sonar_Error = F_sonar_Distance - F_sonar_Distance_old;
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
  
int line_reading()
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

if(sum == 0) line_index = -10; 

if(sum == 1)
{
  if(line_sensor[0]==1) line_index = -4;
  if(line_sensor[1]==1) line_index = -2;
  if(line_sensor[2]==1) line_index =  0;
  if(line_sensor[3]==1) line_index =  4;
  if(line_sensor[4]==1) line_index =  2;
}

if(sum == 2)
{
  if((line_sensor[0]==1) && (line_sensor[1]==1) ) line_index = -3;
  if((line_sensor[1]==1) && (line_sensor[2]==1) ) line_index = -1;
  if((line_sensor[2]==1) && (line_sensor[4]==1) ) line_index =  1;
  if((line_sensor[3]==1) && (line_sensor[4]==1) ) line_index =  3;
}

Serial.print(line_index);
Serial.println("");
return line_index;

}

void line_following()
{
  int index;
  index = line_reading();
    switch(index)
    {
      
    case  0 : //전진
           motor_control(1,150,1,150);
           Serial.print("전진");
           break;

    case 1 : //약 우회전
            motor_control(1,150,1,40);
            Serial.print("약 우회전");
            break;
            
    case 2 : //우회전
            motor_control(1,200,1,40);
            Serial.print("우회전");
            break;
    
    case 3 : //급 우회전
            motor_control(1,200,1,20);
            Serial.print("급 우회전");
            break;

    case 4 : //직각 우회전
            motor_control(1,255,1,20);
            Serial.print("직각 우회전");
            break;

    case -1: //약 좌회전
            motor_control(1,40,1,150);
            Serial.print("약 좌회전");
            break;
            
    case -2: //좌회전
            motor_control(1,40,1,200);
            Serial.print("좌회전");
            break;

    case -3 : //급 좌회전
            motor_control(1,20,1,200);
            Serial.print("급 좌회전");
            break;
            
    case -4 : //직각 좌회전
            motor_control(1,20,1,255);
            Serial.print("직각 좌회전");
            break;      

    case  -10 ://정지
           motor_control(1,0,1,0);
           Serial.println("정지");
           break;
            
    }
}


void left_wall_following(int motor_diff_L)
{
  read_sonar_sensor();
  updata_sonar_error();
  updata_sonar_old();
  Serial.print("L_sonar : ");Serial.print(L_sonar_Distance); Serial.print("   "); //현재값
  Serial.print("L_sonar_Error : ");Serial.println(L_sonar_Error); //이동 후 거리간격 -면 전보다 벽에 가까워짐
  if((L_sonar_Distance < R_sonar_Distance)||(L_sonar_Distance < L_sonar_Distance_old)) motor_control(1, 100 - motor_diff_L, 1, 60 + motor_diff_L);
}


void motor_tracking() // 양쪽벽 트레킹
{
  read_sonar_sensor();
  updata_sonar_error();
  updata_sonar_old();
  int motor_diff_R = R_sonar_Error * p_gain;// 각도 = -차이값*상수
  int motor_diff_L = L_sonar_Error * p_gain;
  Serial.print("L_sonar : "); Serial.print(L_sonar_Distance); Serial.print("   "); //현재값 모니터 출력
  Serial.print("R_sonar : "); Serial.print(R_sonar_Distance); Serial.println(""); //현재값 모니터 출력
 
  Serial.print("L_sonar_Error : "); Serial.println(L_sonar_Error); //차이값 모니터 출력
  Serial.print("R_sonar_Error : "); Serial.println(R_sonar_Error); //차이값 모니터 출력

  if((L_sonar_Distance < R_sonar_Distance)||(L_sonar_Distance_old < R_sonar_Distance_old)) motor_control(1, 100 - motor_diff_L, 1, 60 + motor_diff_L); //(L<R) 오른쪽으로 턴
  else if(R_sonar_Distance < L_sonar_Distance) motor_control(1, 60 + motor_diff_R, 1, 100 - motor_diff_R); // R<L or D < Old 왼으로 턴
}


void cwrotation()//회전
{
  double min_dg = 0.0; //각도 측정 최솟값
  
   read_sonar_sensor(); //소나 읽고
   if(F_sonar_Distance <= MaxDistance) min_dg = F_sonar_Distance; //500 거리 이하 = 최소거리 ok
   else min_dg = MaxDistance; // 
   
   Serial.println(F_sonar_Distance);
   motor_control(1,0,1,0);
   delay(100);
   Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
   Serial.print("min_dg : "); Serial.println(min_dg);    //최소값 추출

   if(L_sonar_Distance < R_sonar_Distance) // L < R or 700
   {
    while(1)
    {  
      motor_control(1,140,-1,120);
      read_sonar_sensor();
         if((min_dg - 40 < L_sonar_Distance)&&(L_sonar_Distance < min_dg + 35))
         {
          delay(100);
            Serial.println(L_sonar_Distance); 
            Serial.println("왼쪽이 더먼저 최소값에 도달하였습니다. 정지합니다.");     //정상작동할시 뜨는 문구
            Serial.println(" ");
         motor_control(1,0,-1,0);
         break;
         }    
    }
   }
   else if(R_sonar_Distance < L_sonar_Distance) //R < L or 700
   {
    while(1)
   {  
      motor_control(-1,120,1,120);
      read_sonar_sensor();
         if((min_dg - 40 < R_sonar_Distance)&&(R_sonar_Distance < min_dg + 35))
         {
          delay(100);
            Serial.println(R_sonar_Distance); 
            Serial.println("오른쪽이 더먼저 최소값에 도달하였습니다. 정지합니다.");     //정상작동할시 뜨는 문구
            Serial.println(" ");
         motor_control(-1,0,1,0);
         break;
         }    
    }
   }
}



void obstacle_detection() //장애물 감지
{ 
  Serial.print("\nF_sonar : "); Serial.println(F_sonar_Distance);
  while(1)
  {
    read_sonar_sensor();
    if(F_sonar_Distance <= 250)
     {
      Serial.println("obstacle");
      motor_control(0,0,0,0);
      delay(500);                                                           
     }
    else if(F_sonar_Distance > 250)
    break;
  }
}

/*
int wall_detection() //미로 진입시 벽 감지
{
  Serial.print(" measure_width = "); Serial.println(measure_width);
  if(mazor_width*0.9 - mazor_width_tolerance < measure_width < mazor_width*1.1 + mazor_width_tolerance)
      {
       Serial.print(" measure_width = "); Serial.println(measure_width); return 1;
      }
      
}*/



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
  mission_flag = 0;
}

void loop()
{
  
  Serial.print(" replay ");
  if(mission_flag == 0)
  { 
    while(1) // 라인 트레킹
    { Serial.print(" 0 -> ");
      line_reading();
      line_following();
      read_sonar_sensor();
      obstacle_detection();//초음파 센서가 장애물이 있으면 멈추고 없으면 이동
      if(L_sonar_Distance < 700){mission_flag++; break;} // 센서가 0값이 아니면(거리가 길어 감지x) 1이동
     }
  }

  // 1, 미로 진입
  else if(mission_flag == 1){
      Serial.print(" 1 -> ");
    while(1){
      Serial.println(" 1-1 -> ");
      line_reading();
      int i = line_reading();
      read_sonar_sensor();
      if(i != -10 ){mission_flag--; break;} //라인 있으면 타기
      else if((R_sonar_Distance < L_sonar_Distance)||(L_sonar_Distance < R_sonar_Distance))//라인 없으면 벽타기
      {
        motor_tracking();
        read_sonar_sensor();
        Serial.println(" 1-2 -> ");Serial.print("F_sonar :  ");Serial.println(F_sonar_Distance);
        if(F_sonar_Distance < 600)//벽타다가 정면<500일때 턴으로 이동
         {motor_control(1,0,1,0); mission_flag++; break;}
      }
    }
   }

  else if(mission_flag == 2){Serial.println(" 2 -> ");
     cwrotation(); //---->다시 flag = 1
     mission_flag--;
    }

}
