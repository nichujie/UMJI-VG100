#include <Servo.h>
#define VEX_MOTOR 5
#define SERVO_PIN 10
#define BUTTON 3
#define TEST

Servo arm;

bool inited = false;
int cnt = 0;
int buttonLastState = 1;
int buttonState = 1;
int state = 0;
// 0 idle
// 1 deploying (button < 1s)
// 2 retractring (button > 1.5s)

void fang(int t){
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  analogWrite(VEX_MOTOR, 150);
  delay(t);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void shou(int t){
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  analogWrite(VEX_MOTOR, 150);
  delay(t);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void setup() {
  Serial.begin(9600);
  Serial.println("initializing...");
  arm.attach(SERVO_PIN);
  arm.writeMicroseconds(1800);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(VEX_MOTOR, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}


void loop() {
  buttonLastState = buttonState;
  buttonState = digitalRead(BUTTON);

  if(!buttonState) {
    cnt++;
  }

  if(buttonState != buttonLastState) {
    if(buttonState) {
      if(cnt < 300) {
        state = 1;
      } else if(cnt > 500) {
        state = 2;
      }
      cnt = 0;
    } else {
      cnt = 0;
    }
  }
  
  if (state == 1){
    arm.writeMicroseconds(1050);
    delay(500);
    shou(1000);
    delay(200);
    arm.writeMicroseconds(850);
    delay(500);
    state = 0;
  }
  else if (state == 2) {
    arm.writeMicroseconds(1300);
    shou(3000);
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    arm.writeMicroseconds(1800);
    state = 0;
  }
  Serial.println(state);
}