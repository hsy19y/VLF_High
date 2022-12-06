#define Obstancle 20

#define IN1 3 
#define IN2 2 
#define IN3 4
#define IN4 7

#define ENA 5
#define ENB 6



int line_sensor[5] = {0,0,0,0,0};

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
} // 라인 센서 구동함수

int a;

void setup(){
 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  analogWrite(ENA, HIGH);
  analogWrite(ENA, HIGH);
  Serial.begin(9600);
  delay(a);
  }

void moter_control(int dir_l, int speed_1, int dir_r, int speed_2)
{
 if(dir_l == 1) //전진
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed_1);
 }
 else if(dir_l == -1)//후진
  {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed_1);
  } 
 else
 {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
 }

 if(dir_r == 1)
 {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed_2);
 }
 else if(dir_r == -1)
 {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed_2);
 }
 else
 {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
 }

}

void loop()
{
  read_line_sensor();
  int index;
  index = read_line_sensor();
  

  switch(index)
  {
    case  -10 ://정지
           moter_control(1,0,1,0);
           break;
           
    case  0 : //전진
           moter_control(1,100,1,100);
           break;

    case 1 : //좌회전
            moter_control(1,70,1,100);
            break;

    case 3 : //급 좌회전
            moter_control(1,50,1,110);
            break;

    case 4 : //직각 좌회전
            moter_control(1,30,1,120);
            break;

    case -1: //우회전
            moter_control(1,100,1,70);
            break;

    case -3 : //급 우회전
            moter_control(1,110,1,50);
            break;
            
    case -4 : //직각 우회전
            moter_control(1,30,1,120);
            break;        
   
  }

}
