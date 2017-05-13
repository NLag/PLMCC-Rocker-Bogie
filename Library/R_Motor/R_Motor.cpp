#include "Arduino.h"
#include "R_Motor.h"

R_Motor::R_Motor(int pa, int pb, int en)
{
  pinA = pa;
  pinB = pb;
  Enb = en;
  //pin control direction and rotation of motor
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(Enb, OUTPUT);
  //initialize pin motors to 0 => no movements
  max_speed = 0;
  speed_motor = 0;
  rotation = 0;
  acceleration = 0;
  digitalWrite(pinA, 0);
  digitalWrite(pinB, 0);
  analogWrite(Enb, 0);
}

void R_Motor::setting(int rotate, int max_spd, int accel) {
  max_speed = max_spd;
  acceleration = accel;
  rotation = rotate;
  if (rotation == 1)
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);  
  }
  else{
    if (rotation == 0)
    {
      digitalWrite(pinA,LOW);
      digitalWrite(pinB,LOW);
    }
    else if (rotation == -1){
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);   
    }
  }
}

void R_Motor::accelerate()
{
  if (speed_motor < max_speed)
  {
    speed_motor += acceleration;  
  }
  analogWrite(Enb, speed_motor);
}

void R_Motor::deccelerate() {
  speed_motor = 0;
  analogWrite(Enb, speed_motor); 
}

bool R_Motor::isMaxspeed_motor() {
  return (speed_motor == max_speed);
}

void R_Motor::reset()
{
  max_speed = 0;
  speed_motor = 0;
  rotation = 0;
  acceleration = 0;
  digitalWrite(pinA, 0);
  digitalWrite(pinB, 0);
  analogWrite(Enb, 0);
}

int R_Motor::whatisspeed() {
  return speed_motor;
}