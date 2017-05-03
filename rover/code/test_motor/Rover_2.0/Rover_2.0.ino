#include <Servo.h>
//motors
//2 head motors
//in A, in B
#define A_1 22
#define B_1 23
#define C_1 24
#define D_1 25
//En A, En B
#define EnA_1 2
#define EnB_1 3

//2 middle motors
//in A, in B
#define A_2 26
#define B_2 27
#define C_2 28
#define D_2 29
//En A, En B
#define EnA_2 4
#define EnB_2 5

// 2 rear motors
//in A, in B
#define A_3 30
#define B_3 31
#define C_3 32
#define D_3 33
//En A, En B
#define EnA_3 6
#define EnB_3 7

//servo
#define tr 8
#define br 9
#define bl 10
#define tl 11

Servo ser_tr, ser_br, ser_bl, ser_tl; //tr = top right, br = back right, bl = back left, tl = top left, 
int speed = 0;          //control the speed of the motors
int direction = 2;      //direction = 1 go straight, 0 go backwards, -1 tank rotation
int control_signal = 0; 

//control the speed of the motor
void speed_up(int m1, int m2, int m3, int m4, int m5, int m6){
    analogWrite(m1, speed);     //write speed to a motor
    analogWrite(m2, speed);
    analogWrite(m3, speed);  
    analogWrite(m4, speed);
    analogWrite(m5, speed);  
    analogWrite(m6, speed);
    delay(500);     //wait half a second 
    if (speed < 25)     //if the speed > 25, maintain the speed
    {
        ++speed;
    }
}

//indicate the direction of movement
void move(int control_signal){
    if (direction != 1 && (control_signal == 8 || control_signal == 9 || control_signal == 10))     //not straight then set it to straight
    {
        speed = 0;
        direction = 1;
    }
    else if (direction != 0 && (control_signal == 4 || control_signal == 5 || control_signal ==6))  
    {
        speed = 0;
        direction = 0;
    }
    else if (direction != -1 && (control_signal == 1 || control_signal == 2))
    {
        speed = 0;
        direction = -1;
    }
    speed_up(EnA_1, EnB_1, EnA_2, EnB_2, EnA_3, EnB_3);
}

//control the high and low current into the L298
void motor(int hi, int lo){
    digitalWrite(hi, 1);
    digitalWrite(lo, 0);
}

//function to control the angle of the servo
void steer(int ang_right, int ang_left){
    //ang_x = angle of each servo
    ser_tr.write(ang_right);
    ser_br.write(ang_right);
    ser_bl.write(ang_left);
    ser_tl.write(ang_left);
}

void setup()
{   
    //setup pins
    Serial.begin(9600);
    //pin control direction of L298
    //head
    pinMode(A_1, OUTPUT);     
    pinMode(B_1, OUTPUT);    
    pinMode(C_1, OUTPUT);    
    pinMode(D_1, OUTPUT);
    pinMode(EnA_1, OUTPUT);
    pinMode(EnB_1, OUTPUT);   
    //mid
    pinMode(A_2, OUTPUT);     
    pinMode(B_2, OUTPUT);    
    pinMode(C_2, OUTPUT);    
    pinMode(D_2, OUTPUT);
    pinMode(EnA_2, OUTPUT);
    pinMode(EnB_2, OUTPUT);
    //rear
    pinMode(A_3, OUTPUT);     
    pinMode(B_3, OUTPUT);    
    pinMode(C_3, OUTPUT);    
    pinMode(D_3, OUTPUT);
    pinMode(EnA_3, OUTPUT);
    pinMode(EnB_3, OUTPUT);
    //initialize pin motors to 0 => no movements
    //head
    digitalWrite(A_1, 0);
    digitalWrite(B_1, 0);
    digitalWrite(C_1, 0);
    digitalWrite(D_1, 0);
    analogWrite(EnA_1, 0);
    analogWrite(EnB_1, 0);
    //mid
    digitalWrite(A_2, 0);
    digitalWrite(B_2, 0);
    digitalWrite(C_2, 0);
    digitalWrite(D_2, 0);
    analogWrite(EnA_2, 0);
    analogWrite(EnB_2, 0);
    //rear
    digitalWrite(A_3, 0);
    digitalWrite(B_3, 0);
    digitalWrite(C_3, 0);
    digitalWrite(D_3, 0);
    analogWrite(EnA_3, 0);
    analogWrite(EnB_3, 0);
    //set up servos 
    ser_tr.attach(tr);   //top right servo
    ser_br.attach(br);   //back right servo
    ser_bl.attach(bl);   //back left servo
    ser_tl.attach(tl);   //top left servo
    //initialize angles of the servos at 0 degree
    steer(0, 0);
    speed = 0; 
    direction = 2;      //not in a particular direction
    control_signal = 0;
}

void loop()
{
    if (Serial.read() != control_signal)    //only change the movement when input signal is different
    {
        control_signal = Serial.read();     
    }
    switch(control_signal){
        case 8:     //go straight
            steer(0, 0);
            move(control_signal);
            //2 head motors
            motor(A_1, B_1);
            motor(C_1, D_1);
            //2 middle motors
            motor(A_2, B_2);
            motor(C_2, D_2);
            //2 rear motors
            motor(A_3, B_3);
            motor(C_3, D_3);
            //move();
            break;
        case 9:     //turn right straight 
            steer(45, 18);
            move(control_signal);
            //head
            motor(A_1, B_1);
            motor(C_1, D_1);
            //middle
            motor(A_2, B_2);
            motor(C_2, D_2);
            //rear
            motor(A_3, B_3);
            motor(C_3, D_3);
            //move();
            break;
        case 10:    //turn left straight
            steer(18, 45);
            move(control_signal);
            //head
            motor(A_1, B_1);
            motor(C_1, D_1);
            //middle
            motor(A_2, B_2);
            motor(C_2, D_2);
            //rear
            motor(A_3, B_3);
            motor(C_3, D_3);
            //move();
            break;
        case 4:     //go backward
            steer(0, 0);
            move(control_signal);
            //head
            motor(B_1, A_1);
            motor(D_1, C_1);
            //middle
            motor(B_2, A_2);
            motor(D_2, C_2);
            //rear
            motor(B_3, A_3);
            motor(D_3, D_3);
            //move();
            break;
        case 5:     //turn right backward
            steer(45, 18);
            move(control_signal);
            //head
            motor(B_1, A_1);
            motor(D_1, C_1);
            //middle
            motor(B_2, A_2);
            motor(D_2, C_2);
            //rear
            motor(B_3, A_3);
            motor(D_3, D_3);
            //move();
            break;
        case 6:     //turn left backward 
            steer(18, 45);
            move(control_signal);
            //head
            motor(B_1, A_1);
            motor(D_1, C_1);
            //middle
            motor(B_2, A_2);
            motor(D_2, C_2);
            //rear
            motor(B_3, A_3);
            motor(D_3, D_3);
            //move();
            break;
        case 1:    //tank rotation right
            steer(0, 0);
            move(control_signal);
            //head
            motor(A_1, B_1);
            motor(D_1, C_1);
            //middle
            motor(A_2, B_2);
            motor(D_2, C_2);
            //rear
            motor(A_3, B_3);
            motor(D_3, C_3);
            //move();
            break;
        case 2:     //tank rotation left
            steer(0, 0);
            move(control_signal);
            //head
            motor(B_1, A_1);
            motor(C_1, D_1);
            //middle
            motor(B_2, A_2);
            motor(C_2, D_2);
            //rear
            motor(B_3, A_3);
            motor(C_3, D_3);
            //move();

        default:
            speed = 0;
            //head
            digitalWrite(A_1, 0);
            digitalWrite(B_1, 0);
            digitalWrite(C_1, 0);
            digitalWrite(D_1, 0);
            analogWrite(EnA_1, 0);
            analogWrite(EnB_1, 0);
            //mid
            digitalWrite(A_2, 0);
            digitalWrite(B_2, 0);
            digitalWrite(C_2, 0);
            digitalWrite(D_2, 0);
            analogWrite(EnA_2, 0);
            analogWrite(EnB_2, 0);
            //rear
            digitalWrite(A_3, 0);
            digitalWrite(B_3, 0);
            digitalWrite(C_3, 0);
            digitalWrite(D_3, 0);
            analogWrite(EnA_3, 0);
            analogWrite(EnB_3, 0);
            steer(0, 0);
    }
}
