
#include <NewPing.h>
#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

#define SONAR_NUM 3 //number of sensor
#define MaxDistance 600  //Maxium distance (in cm) to ping.

#define IN1 2 //left
#define IN2 3 
#define IN3 4 //right
#define IN4 7

#define ENL 5 //left
#define ENR 6 //right

int robot_width = 200; // 로봇의 너비 
int mazor_width = 500; // 미로의 폭
int mazor_width_tolerance = 10; // 센서의 오차범위
int measure_width; // 총 산출값
int DF_3 = 250;

int a;
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

void moter(int i, int L_Ms, int R_Ms)
{
  switch(i)
  {
    case 1 ://전진
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENL, L_Ms);
    analogWrite(ENR, R_Ms);
    break;
  
    case 2 ://후진
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENL, L_Ms);
    analogWrite(ENR, R_Ms);
    break;

    case 3 ://우회전
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW);
    analogWrite(ENL, L_Ms);
    analogWrite(ENR, R_Ms);
    break;

    case 4 ://좌회전
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENL, L_Ms);
    analogWrite(ENR, R_Ms);
    break;

    case 5 ://잠시 정지
    analogWrite(ENL,0);
    analogWrite(ENR,0);
    delay(1000);
    break;
  }
}

int read_line_sensor()
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

if((sum == 5)||(sum == 0))

  line_index = -10; 

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

Serial.print("line_index",line_index);
Serial.println("");
return line_index;

}

 

void cwrotation()//회전
{
  int i;
  double min_dg = 0.0; //각도 측정 최솟값
  double min_dg2 = 0.0; 
  min_dg = F_sonar_Distance;
  for(i=0; i<400; i++)
  {   
    moter(1,90,90);
        
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
   moter(1,0,0);
   delay(1000);
   Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
   Serial.println(min_dg);    //최소값 추출 완료
   Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
   
   while(1)
   {     
         moter(3,90,90);
 
    
  
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
          moter(3,0,0);
          break;
         }
         
         else if(R_sonar_Distance < min_dg + 1 && R_sonar_Distance > min_dg - 1)
         {
          delay(250);
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"); 
            Serial.println("오른쪽이 더먼저 최소값에 도달하였습니다. 정지합니다.");     //정상작동할시 뜨는 문구
            Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
          moter(3,0,0);
          break;
         }
  }
}


void left_wall_following(int moter_diff_L)//
{
  
  if(L_sonar_Distance < R_sonar_Distance)
  {
     moter(1, 140 + moter_diff_L, 140 - moter_diff_L);
  }
  
}


void moter_tracking(int moter_diff_L, int moter_diff_R)//양쪽벽 트레킹
{
  
  if((R_sonar_Distance < L_sonar_Distance)&&(R_sonar_Distance < 200))
  {
    moter(1, 30 - moter_diff_R, 180 + moter_diff_R); //오른족으로
  }   
  else if((L_sonar_Distance < R_sonar_Distance)&&(L_sonar_Distance < 200))
  {
    moter(1, 140 + moter_diff_L, 100 - moter_diff_L);
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

  read_line_sensor();
  int index;
  index = read_line_sensor();
  
  do{
    switch(index){
    
    case  -10 ://정지
           moter(1,0,0);
           printf("정지");
           break;
           
    case  0 : //전진
           moter(1,60,60);
           printf("전진");
           break;

    case 1 : //좌회전
            moter(1,70,100);
            printf("좌회전");
            break;

    case 3 : //급 좌회전
            moter(1,50,110);
            printf("급 좌회전");
            break;

    case 4 : //직각 좌회전
            moter(1,30,120);
            printf("직각 좌회전");
            break;

    case -1: //우회전
            moter(1,100,70);
            printf("우회전");
            break;

    case -3 : //급 우회전
            moter(1,110,50);
            printf("급 우회전");
            break;
            
    case -4 : //직각 우회전
            moter(1,30,120);
            printf("직각 우회전");
            break;        
   }
  }while(index != -10);
 

   
            
 float p_gain = 3;
 int moter_diff_R, moter_diff_L = 0;
 
 read_sonar_sensor(); // 센서 읽고
 updata_sonar_error(); //차이값을 구하고
 updata_sonar_old();//현재값->old값

 measure_width = L_sonar_Distance + robot_width + R_sonar_Distance; //총 산출값 = 
 
 Serial.print("L_sonar : ");
 Serial.print(L_sonar_Distance); Serial.print("   "); //현재값 모니터 출력
 Serial.print("R_sonar : ");
 Serial.print(R_sonar_Distance); Serial.println(""); //현재값 모니터 출력
 
 Serial.print("L_sonar_Error : ");Serial.println(L_sonar_Error); //차이값 모니터 출력
 Serial.print("R_sonar_Error : ");Serial.println(R_sonar_Error); //차이값 모니터 출력

 moter_diff_R = R_sonar_Error * p_gain;// 각도 = 차이값*상수
 moter_diff_L = L_sonar_Error * p_gain;

  if((index == -10)&&(measure_width*0.9 - mazor_width_tolerance < measure_width < measure_width*1.0 + mazor_width_tolerance)) //벽 진입확인 
  { 
    left_wall_following(moter_diff_L);
    delay(250);
    if((F_sonar_Distance <= DF_3)) //라인 인식x, 거리 250 만큼 이동
    {
      cwrotation();
      left_wall_following(moter_diff_L);
      delay(250);
      if((R_sonar_Distance <= L_sonar_Distance)&&(measure_width*0.9 - mazor_width_tolerance < measure_width < measure_width*1.0 + mazor_width_tolerance)) //
      {
        moter_tracking(moter_diff_L, moter_diff_R);
        delay(250);
      }
    
    }
    
 

  }
}




  
