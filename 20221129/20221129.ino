//3,4 번 핀은 모터제어에 관한 핀
#define IN1 3 //
#define IN2 2 //

#define IN3 4
#define IN4 7

//5,6 pin 모터의 힘을 설정
#define ENA 5

#define ENB 6



void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
}


void loop() {

digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENA, 130); //5번 핀에 130힘을 설정
analogWrite(ENB, 130); // "
delay(1000);

//후진->HIGH,LOW를 반전
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENA, 130);
analogWrite(ENB, 130);
delay(1000);

//우회전
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, HIGH); 
digitalWrite(IN4, LOW);
analogWrite(ENA, 130);
analogWrite(ENB, 130);
delay(1000);

//좌회전
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENA, 130);
analogWrite(ENB, 130);
delay(1000);

analogWrite(ENA, 0);
analogWrite(ENB, 0);
delay(2000);




}
