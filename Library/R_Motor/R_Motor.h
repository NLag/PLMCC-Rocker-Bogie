#ifndef R_Motor_h
#define R_Motor_h

#include "Arduino.h"

class R_Motor
{
  public:
    R_Motor(int pina, int pinb, int enb);
    void setting(int rotate, int max_spd, int accel);
    void accelerate();
    void decelerate();
    void reset();
    bool isMaxSpeed();
    int whatisspeed();
  private:
    int max_speed;
    int pinA;
    int pinB;
    int Enb;
    int speed_motor;
    int acceleration;
    int rotation;
};

#endif