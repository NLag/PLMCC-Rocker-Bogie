#include <Servo.h>
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

Servo ser_tr, ser_br, ser_bl, ser_tl; //tr = top right, br = back right, bl = back left, tl = top left, 
int speed_motor = 0;          //control the speed_motor of the motors
int direction_motor = 2;      //direction_motor = 1 go straight, 0 go backwards, -1 tank rotation
int control_signal = 0; 

//control the speed_motor of the motor
void speed_motor_up(int m1, int m2, int m3, int m4, int m5, int m6){
    analogWrite(m1, speed_motor);     //write speed_motor to a motor
    analogWrite(m2, speed_motor);
    analogWrite(m3, speed_motor);  
    analogWrite(m4, speed_motor);
    analogWrite(m5, speed_motor);  
    analogWrite(m6, speed_motor);
    delay(200);     //wait half a second 
    if (speed_motor < 25)     //if the speed_motor > 25, maintain the speed_motor
    {
        ++speed_motor;
    }
}

//indicate the direction_motor of move_roverment
void move_rover(int control_signal){
    if (direction_motor != 1 && (control_signal == 8 || control_signal == 9 || control_signal == 10))     //not straight then set it to straight
    {
        speed_motor = 0;
        change_direction(1);
    }
    else if (direction_motor != 0 && (control_signal == 4 || control_signal == 5 || control_signal ==6))  
    {
        speed_motor = 0;
        change_direction(0);
    }
    else if (direction_motor != -1 && (control_signal == 1 || control_signal == 2))
    {
        speed_motor = 0;
        change_direction(-control_signal);
    }
    speed_motor_up(EnA_1, EnB_1, EnA_2, EnB_2, EnA_3, EnB_3);
}

void change_direction(int direct) {
    direction_motor = direct;
    switch (direction_motor) {
      case 1:
        //2 head motors
        motor(A_1, B_1);
        motor(C_1, D_1);
        //2 middle motors
        motor(A_2, B_2);
        motor(C_2, D_2);
        //2 rear motors
        motor(A_3, B_3);
        motor(C_3, D_3);
        break;
      case 0:
        //head
        motor(B_1, A_1);
        motor(D_1, C_1);
        //middle
        motor(B_2, A_2);
        motor(D_2, C_2);
        //rear
        motor(B_3, A_3);
        motor(D_3, D_3);
      case -1:
        //head
        motor(A_1, B_1);
        motor(D_1, C_1);
        //middle
        motor(A_2, B_2);
        motor(D_2, C_2);
        //rear
        motor(A_3, B_3);
        motor(D_3, C_3);
        break;
      case -2:
        //head
        motor(B_1, A_1);
        motor(C_1, D_1);
        //middle
        motor(B_2, A_2);
        motor(C_2, D_2);
        //rear
        motor(B_3, A_3);
        motor(C_3, D_3);
        break;
    }
}

//control the high and low current into the L298
void motor(int hi, int lo){
    digitalWrite(hi, HIGH);
    digitalWrite(lo, LOW);
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
    //pin control direction_motor of L298
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
    //initialize pin motors to 0 => no move_roverments
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
    speed_motor = 0; 
    direction_motor = 2;      //not in a particular direction_motor
    control_signal = 0;
}

void loop()
{
    int data_in = Serial.read();
    if (data_in != -1 && data_in != control_signal)
    control_signal = data_in;
    Serial.print(control_signal);
    Serial.print("\n");
    switch(control_signal){
        case 8:     //go straight
            steer(0, 0);
           
            //move_rover();
            move_rover(control_signal);
            
            break;
        case 9:     //turn right straight 
            //steer(45, 18);
            steer(90, 60);
            
            //move_rover();
            move_rover(control_signal);
            break;
        case 10:    //turn left straight
            //steer(18, 45);
            steer(60, 90);
       
            //move_rover();
            move_rover(control_signal);
            break;
        case 4:     //go backward
            steer(0, 0);
            
            //move_rover();
            move_rover(control_signal);
            break;
        case 5:     //turn right backward
            //steer(45, 18);
            steer(90, 60);
            
            //move_rover();
            move_rover(control_signal);
            break;
        case 6:     //turn left backward 
            //steer(18, 45);
            steer(60, 90);
            
            //move_rover();
            move_rover(control_signal);
            break;
        case 1:    //tank rotation right
            steer(0, 0);
            
            //move_rover();
            move_rover(control_signal);
            break;
        case 2:     //tank rotation left
            steer(0, 0);
            
            
            //move_rover();
            move_rover(control_signal);
            break;
        default:
            speed_motor = 0;
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
            delay(200);
    }
}
