#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial ble(10, 11); 

Servo myservo;
int Left = 200;
int Right = 200;
int Trig = A5;
int Echo = A4;
int ENA = 5;
int ENB = 6;
int IN1 = 7;
int IN2 = 8;
int IN3 = 9;
int IN4 = 11;
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

void NeVatCan()
{
  while(true)
  {
    Dung();
    myservo.write (0);
    delay(1000);
    Right = Distance();
    myservo.write (180);
    delay(1000);
    Left = Distance();
    if (Left > Right)
    {
      LuiTrai(450);
      myservo.write(90);
      break;
    }
    else if (Right >= Left)
    {
      LuiPhai(450);
      myservo.write(90);
      break;
    }
    if (Right < 30 and Left < 30) 
    {
      DiLui(2000);
      Do();
      break;
    }
  }
}
void Do()
{
  if (LT_M == 1 and (LT_L == 1 or LT_R == 1))
    Dung();
  else
  {
    myservo.write(90-30);
    delay(300);
    if (Distance() < 30)
    NeVatCan();
    myservo.write(90);
    delay(300);
    if (Distance() < 30)
    NeVatCan();
    myservo.write(90+30);
    delay(300);
    if (Distance() < 30)
    NeVatCan();
  }
}
void DiThang()
{
  analogWrite(ENA,150);
  analogWrite(ENB,150);
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
}
void Dung()
{
  analogWrite(ENA,0);
  analogWrite(ENB,0);
}
void DiLui(int time)
{
  analogWrite(ENA,150);
  analogWrite(ENB,150);
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
  delay(time);
}
void LuiTrai(int time)
{
  analogWrite(ENA,200);
  analogWrite(ENB,200);
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  delay(time);
}
void LuiPhai(int time)
{
  analogWrite(ENA,200);
  analogWrite(ENB,200);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
  delay(time);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ble.begin(9600); // Bluetooth start
  myservo.attach(3);
  myservo.write(90);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  analogWrite(ENA,150);
  analogWrite(ENB,150);
}
int Distance()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance = Fdistance / 58;
  return (int)Fdistance; 
 }

void loop() {
  myservo.write(90);
  while (LT_M == 0 and (LT_L == 0 or LT_R == 0))
  {
    if (Distance() < 30)
      NeVatCan();
    else
      DiThang();
    Do();
  }
  Dung();
}