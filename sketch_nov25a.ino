#include <Servo.h>

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

char data;
bool isAuto = false;
bool isStarted = false;

int Distance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance = Fdistance / 58;
  return (int)Fdistance;
}

void setup() {
  Serial.begin(9600);

  myservo.attach(3);
  myservo.write(90);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Dung();
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();

    if (data == '5') { 
      if (isStarted) { 
        isStarted = false;
        Dung();
      } else { 
        isStarted = true;
        isAuto = false;
      }
    } else if (data == '6') { 
      isStarted = true;
      isAuto = true;
    } else if (!isAuto && isStarted) {
      ManualControl(data);
    }
  }

  if (isStarted && isAuto) {
    AutoMode();
  }
}

void ManualControl(char command) {
  switch (command) {
    case '1': 
      DiThang();  
      break;
    case '2': 
      DiLui(0);  
      break;
    case '3': 
      ReTrai(0);  
      break;
    case '4': 
      RePhai(0);  
      break;
    case 'stop': 
      Dung();  
      break;
  }
}

void AutoMode() {
  myservo.write(90);  
  while (LT_M == 0 && (LT_L == 0 || LT_R == 0)) {
    if (Distance() < 30)
      NeVatCan();  
    else
      DiThang();  
    Do();
  }
  Dung();
}

void DiThang() {
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Dung() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void DiLui(int time) {
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(time);
}

void ReTrai(int time) { 
  analogWrite(ENA, 150); 
  analogWrite(ENB, 150); 
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
  delay(time); 
}

void RePhai(int time) { 
  analogWrite(ENA, 150); 
  analogWrite(ENB, 150); 
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH); 
  delay(time); 
}

void NeVatCan() {
  while (true) {
    Dung();
    myservo.write(0);
    delay(1000);
    Right = Distance();
    myservo.write(180);
    delay(1000);
    Left = Distance();
    if (Left > Right) {
      ReTrai(450); 
      myservo.write(90);
      break;
    } else if (Right >= Left) {
      RePhai(450); 
      myservo.write(90);
      break;
    }
    if (Right < 30 && Left < 30) {
      DiLui(2000);
      Do();
      break;
    }
  }
}

void Do() {
  if (LT_M == 1 && (LT_L == 1 || LT_R == 1))
    Dung();
  else {
    myservo.write(90 - 30);
    delay(300);
    if (Distance() < 30)
      NeVatCan();
    myservo.write(90);
    delay(300);
    if (Distance() < 30)
      NeVatCan();
    myservo.write(90 + 30);
    delay(300);
    if (Distance() < 30)
      NeVatCan();
  }
}
