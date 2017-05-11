#include <Servo.h>
#include <R_Motor.h>

/*-------------------DEFINE PIN-----------------------------------*/
//motors
//2 head motors
//in A, in B
#define A_1 29
#define B_1 31
#define C_1 33
#define D_1 35
//En A, En B
#define EnA_1 2
#define EnB_1 3

//2 middle motors
//in A, in B
#define A_2 37
#define B_2 39
#define C_2 41
#define D_2 43
//En A, En B
#define EnA_2 4
#define EnB_2 5

// 2 rear motors
//in A, in B
#define A_3 45
#define B_3 47
#define C_3 49
#define D_3 51
//En A, En B
#define EnA_3 6
#define EnB_3 7

//servo
#define tl 8
#define tr 9
#define bl 10
#define br 11

/*--------------------DEFINE VARIABLE----------------------------------*/
//define speed for turning 
#define MAXSPEED 255

#define OutTurnSpeedFRONT 0.97
#define OutTurnSpeedMIDDLE 0.9285
#define OutTurnSpeedREER 1

#define InTurnSpeedFRONT 0.39767
#define InTurnSpeedMIDDLE 0.2812
#define InTurnSpeedREER 0.466

#define CenterSpeedFRONT 0.42874
#define CenterSpeedMIDDLE 0.3236
#define CenterSpeedREER 0.4926

//define angle for turning (degree)
#define DEFAULTANGLE 90 			
#define OutTurnAngleFRONT 16.85
#define OutTurnAngleREER 21.8

#define InTurnAngleFRONT 45
#define InTurnAngleREER 52.87

#define CenterAngleFRONT 40.986
#define CenterAngleREER 48.93

//define speed for self-rotate

//define speed

/*------------------------DEFINE SERVO and MOTOR------------------------------*/
//t = top, m = middle, b = back, l = left, r = right


//setup motor
R_Motor motor_tl(A_1,B_1,EnA_1);
R_Motor motor_ml(A_2,B_2,EnA_2);
R_Motor motor_bl(A_3,B_3,EnA_3);
R_Motor motor_tr(C_1,D_1,EnB_1);
R_Motor motor_mr(C_2,D_2,EnB_2);
R_Motor motor_br(C_3,D_3,EnB_3);

//setup servo
Servo ser_tr, ser_br, ser_tl, ser_bl; 
int max_angle_tr,max_angle_br,max_angle_tl,max_angle_bl;
int accel_tr,accel_br,accel_tl,accel_bl;

int standstil = 1; 
int steering = 0; 
int moving = 0;
int control_signal = 0;


/*--------------------------CONTROL CAR to RUN----------------------------*/

bool speed_motor_down() {
	motor_tl.decelerate();
	motor_ml.decelerate();
	motor_bl.decelerate();
	motor_tr.decelerate();
	motor_mr.decelerate();
	motor_br.decelerate();
}

void speed_motor_up() {
	motor_tl.accelerate();
	motor_ml.accelerate();
	motor_bl.accelerate();
	motor_tr.accelerate();
	motor_mr.accelerate();
	motor_br.accelerate();
} 

void servoSteer(Servo ser, int max_angle, int angle_accel)
{
	int angle = ser.read();
	if (angle < max_angle)
  	{
    	angle += angle_accel;
    	if (angle > max_angle)	
    	{
    		angle = max_angle;
    	}
  	}
  	ser.write(angle);
}

void steer() {
    servoSteer(ser_tr, max_angle_tr, accel_tr);
    servoSteer(ser_br, max_angle_br, accel_br);
    servoSteer(ser_tl, max_angle_tl, accel_tl);
    servoSteer(ser_bl, max_angle_bl, accel_bl);
}

bool steerDone() {
	return ((ser_tr.read() == max_angle_tr)&&(ser_br.read() == max_angle_br)&&(ser_tl.read() == max_angle_tl)&&(ser_bl.read() == max_angle_bl));
}

bool toMaxSpeed() {
	return (motor_tr.isMaxSpeed()&&motor_br.isMaxSpeed()&&motor_tl.isMaxSpeed()&&motor_bl.isMaxSpeed());
}
/*-------------------------------------------------*/
void controlCar(){
	if (standstil == 1)
	{
		speed_motor_down();
		standstil = 0;
		steering = 1;
	}
	else {
		if (steering == 1)
		{
			steer();
			if (steerDone())
			{
				steering = 0;
				moving = 1;	
			}
		}
		else {
			if (moving = 1)
			{
				speed_motor_up();
				if (toMaxSpeed())
				{
					moving = 0;
				}
			}
		}	
	}
	delay(10);
}
/*-------------------------PROPERTIES for MOTOR and SERVO-----------------------------*/
// Based on rover 
void goStraight(){
	motor_tr.setting(1 , MAXSPEED , 1); //setting(rotation, speed, acceleration)
	motor_mr.setting(1 , MAXSPEED , 1);
	motor_br.setting(1 , MAXSPEED , 1);
	motor_tl.setting(1 , MAXSPEED , 1);
	motor_ml.setting(1 , MAXSPEED , 1);
	motor_bl.setting(1 , MAXSPEED , 1);
	max_angle_tl = DEFAULTANGLE;
	max_angle_tr = DEFAULTANGLE;
	max_angle_bl = DEFAULTANGLE;
	max_angle_br = DEFAULTANGLE;
}
void goRightStraight(){
	motor_tr.setting(1 , 255*InTurnSpeedFRONT , InTurnSpeedFRONT);
	motor_mr.setting(1 , 255*InTurnSpeedMIDDLE , InTurnSpeedMIDDLE);
	motor_br.setting(1 , 255*InTurnSpeedREER , InTurnSpeedREER);
	motor_tl.setting(1 , 255*OutTurnSpeedFRONT , OutTurnSpeedFRONT );
	motor_ml.setting(1 , 255*OutTurnSpeedMIDDLE , OutTurnSpeedMIDDLE);
	motor_bl.setting(1 , 255*OutTurnSpeedREER , OutTurnSpeedREER);
	max_angle_tr = DEFAULTANGLE + InTurnAngleFRONT;
	max_angle_br = DEFAULTANGLE - InTurnAngleREER;	
	max_angle_tl = DEFAULTANGLE + OutTurnAngleFRONT;
	max_angle_bl = DEFAULTANGLE - OutTurnAngleREER;
	
}
void goLeftStraight(){
	motor_tr.setting(1 , 255*OutTurnSpeedFRONT , OutTurnSpeedFRONT);
	motor_mr.setting(1 , 255*OutTurnSpeedMIDDLE , OutTurnSpeedMIDDLE);
	motor_br.setting(1 , 255*OutTurnSpeedREER , OutTurnSpeedREER);
	motor_tl.setting(1 , 255*InTurnSpeedFRONT , InTurnSpeedFRONT);
	motor_ml.setting(1 , 255*InTurnSpeedMIDDLE , InTurnSpeedMIDDLE);
	motor_bl.setting(1 , 255*InTurnSpeedREER , InTurnSpeedREER);
	max_angle_tr = OutTurnAngleFRONT;
	max_angle_br = OutTurnAngleREER;	
	max_angle_tl = InTurnAngleFRONT;
	max_angle_bl = InTurnAngleREER;
}
void goBackward(){
	motor_tr.setting(-1 , MAXSPEED , 1);
	motor_mr.setting(-1 , MAXSPEED , 1);
	motor_br.setting(-1 , MAXSPEED , 1);
	motor_tl.setting(-1 , MAXSPEED , 1);
	motor_ml.setting(-1 , MAXSPEED , 1);
	motor_bl.setting(-1 , MAXSPEED , 1);
	max_angle_tl = 0;
	max_angle_tr = 0;
	max_angle_bl = 0;
	max_angle_br = 0;
}
void goRightBackward(){
	motor_tr.setting(1 , 255*InTurnSpeedFRONT , InTurnSpeedFRONT);
	motor_mr.setting(1 , 255*InTurnSpeedMIDDLE , InTurnSpeedMIDDLE);
	motor_br.setting(1 , 255*InTurnSpeedREER , InTurnSpeedREER);
	motor_tl.setting(1 , 255*OutTurnSpeedFRONT , OutTurnSpeedFRONT );
	motor_ml.setting(1 , 255*OutTurnSpeedMIDDLE , OutTurnSpeedMIDDLE);
	motor_bl.setting(1 , 255*OutTurnSpeedREER , OutTurnSpeedREER);
	max_angle_tr = InTurnAngleFRONT;
	max_angle_br = InTurnAngleREER;	
	max_angle_tl = OutTurnAngleFRONT;
	max_angle_bl = OutTurnAngleREER;
}
void goLeftBackward(){
	motor_tr.setting(-1 , 255*OutTurnSpeedFRONT , OutTurnSpeedFRONT);
	motor_mr.setting(-1 , 255*OutTurnSpeedMIDDLE , OutTurnSpeedMIDDLE);
	motor_br.setting(-1 , 255*OutTurnSpeedREER , OutTurnSpeedREER);
	motor_tl.setting(-1 , 255*InTurnSpeedFRONT , InTurnSpeedFRONT);
	motor_ml.setting(-1 , 255*InTurnSpeedMIDDLE , InTurnSpeedMIDDLE);
	motor_bl.setting(-1 , 255*InTurnSpeedREER , InTurnSpeedREER);
	max_angle_tr = OutTurnAngleFRONT;
	max_angle_br = OutTurnAngleREER;	
	max_angle_tl = InTurnAngleFRONT;
	max_angle_bl = InTurnAngleREER;
}
void rotationRight(){
	motor_tr.setting(-1 , 255*CenterSpeedFRONT , CenterSpeedFRONT);
	motor_mr.setting(-1 , 255*CenterSpeedMIDDLE , CenterSpeedMIDDLE);
	motor_br.setting(-1 , 255*CenterSpeedREER, CenterSpeedREER);
	motor_tl.setting(1 , 255*CenterSpeedFRONT , CenterSpeedFRONT);
	motor_ml.setting(1 , 255*CenterSpeedMIDDLE , CenterSpeedMIDDLE);
	motor_bl.setting(1 , 255*CenterSpeedREER , CenterSpeedREER);
	max_angle_tr = CenterAngleFRONT;
	max_angle_br = CenterAngleREER;	
	max_angle_tl = CenterAngleFRONT;
	max_angle_bl = CenterAngleREER;	
}
void rotationLeft(){
	motor_tr.setting(1 , 255*CenterSpeedFRONT , CenterSpeedFRONT);
	motor_mr.setting(1 , 255*CenterSpeedMIDDLE , CenterSpeedMIDDLE);
	motor_br.setting(1 , 255*CenterSpeedREER, CenterSpeedREER);
	motor_tl.setting(-1 , 255*CenterSpeedFRONT , CenterSpeedFRONT);
	motor_ml.setting(-1 , 255*CenterSpeedMIDDLE , CenterSpeedMIDDLE);
	motor_bl.setting(-1 , 255*CenterSpeedREER , CenterSpeedREER);
	max_angle_tr = CenterAngleFRONT;
	max_angle_br = CenterAngleREER;	
	max_angle_tl = CenterAngleFRONT;
	max_angle_bl = CenterAngleREER;
}
void stopall(){
	motor_tr.setting(0 , 0 , 0);
	motor_mr.setting(0 , 0 , 0);
	motor_br.setting(0 , 0 , 0);
	motor_tl.setting(0 , 0 , 0);
	motor_ml.setting(0 , 0 , 0);
	motor_bl.setting(0 , 0 , 0);
	max_angle_tl = DEFAULTANGLE;
	max_angle_tr = DEFAULTANGLE;
	max_angle_bl = DEFAULTANGLE;
	max_angle_br = DEFAULTANGLE;
}
/*-------------------------------------------------*/
void settingProperties() {
	switch(control_signal){
        case 8:     //go straight
            goStraight();
            break;
        case 9:     //turn right straight 
            goRightStraight();
            break;
        case 10:    //turn left straight
            goLeftStraight();
            break;
        case 4:     //go backward
            goBackward();
            break;
        case 5:     //turn right backward
            goRightBackward();
            break;
        case 6:     //turn left backward 
            goLeftBackward();
            break;
        case 1:    //tank rotation right
            rotationRight();
            break;
        case 2:     //tank rotation left
            rotationLeft();
            break;
        default:
            stopall();
    }
}

/*-------------------------RECEIVE SIGNAL-----------------------------*/

void receiveSignal() {
    int data_in = Serial.read();
    if (data_in != control_signal) {
    	control_signal = data_in;
    	standstil = 1;
    }
}

/*-----------------------MAIN PART-------------------------------*/

void setup()
{   
    //setup pins
    Serial.begin(9600);
    //set up servos 
    ser_tr.attach(tr);   //top right servo
    ser_br.attach(br);   //back right servo
    ser_bl.attach(bl);   //back left servo
    ser_tl.attach(tl);   //top left servo

    //initialize angles of the servos at 0 degree
    ser_tr.write(DEFAULTANGLE);
    ser_br.write(DEFAULTANGLE);
    ser_bl.write(DEFAULTANGLE);
    ser_tl.write(DEFAULTANGLE);
    //initialize all speed at 0
    //not in a particular direction_motor

    motor_tr.reset();
    motor_mr.reset();
    motor_br.reset();
    motor_tl.reset();
    motor_ml.reset();
    motor_bl.reset();

    control_signal = 0;         //no input signal 
}

void loop()
{
    receiveSignal();
    settingProperties();
    controlCar();
}
