#include <Servo.h>
#define VEX_MOTOR 5
#define SERVO_PIN 10
#define BUTTON 11

Servo arm;

int cnt = 0;
int buttonLastState = 1;
int buttonState = 1;

// 0 idle
// 1 deploying (button < 1s)
// 2 retractring (button > 1.5s)
int state = 0;


// loose the rope on the bridge top for t millisecond
void loose_rope(int t){
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  analogWrite(VEX_MOTOR, 150);
  delay(t);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

// tight the rope on the bridge top for t millisecond
void tight_rope(int t){
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  analogWrite(VEX_MOTOR, 150);
  delay(t);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void setup() {
  // initialize all ports
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

  // determine the state of button
  // refer to the "state" variable for pressing time
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
  
  // deploy/retract the bridge according to button state
  if (state == 1){
    arm.writeMicroseconds(1050);
    delay(500);
    tight_rope(900);
    delay(200);
    arm.writeMicroseconds(850);
    delay(500);
    state = 0;
  }
  else if (state == 2) {
    loose_rope(1000);
    arm.writeMicroseconds(1800);
    delay(1000);
    state = 0;
  }
}