// Requirements for Shield
#include <AFMotor.h> //This is for stepper


// CHANNEL READINGS
float ch1; 
float ch2;
float ch3;
float ch4;
float ch5;
float ch6;
float s1;
float s2;


// OUTPUT VALUES
//int hook = 7;
//int L_loco = 8;
//int R_loco = 9;
//int L_bar = 10;
//int R_bar = 11;
//int L_coli = 12;
//int R_coli = 13;

//MAXMINS FOR DIFFERENT CHANNELS
float maxch1 = 1905;
float minch1 = 1157;
float maxch2 = 1830;
float minch2 = 1185;
float maxch3 = 1860;
float minch3 = 1183;
float maxch4 = 1960;
float minch4 = 1170;

// SETUP MOTORS
AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR34_64KHZ);
AF_DCMotor motor4(4, MOTOR34_64KHZ);

//We will use motor 1 and 2 as left and right wheels respectively

int motor1speed=0; //from -255 to 255 (Left wheel)
int motor2speed=0; //from -255 to 255 (Right wheel)
int motor3speed=0; //from -255 to 255
int motor4speed=0; //from -255 to 255

void setup() {

  // MAKE CHANNEL INPUT
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(13,INPUT);

  Serial.begin(9600);

  // MAKE OUTPUTS FOR SERVOS
//  pinMode(hook, OUTPUT);
//  pinMode(L_loco, OUTPUT);
//  pinMode(R_loco, OUTPUT);
//  pinMode(L_bar, OUTPUT);
//  pinMode(R_bar, OUTPUT);
//  pinMode(L_coli, OUTPUT);
//  pinMode(R_coli, OUTPUT);
 
}

//Normalization
float normalise(float ch, float minch, float maxch){      
  //convert all the values for right stick to between 0 and 1
  float normch = (ch-minch)/(maxch-minch);
  
  //convert again to -1 and 1
  normch = (normch - 0.5)*2;
  return normch;
}

//Keep motor speeds within the values
void speedlimit(int motorspeed){   
  if (motorspeed >= 255){
    motorspeed = 255;
  } else if (motorspeed <= -255) {
    motorspeed = -255;
  } else if (motorspeed <= 15 and motorspeed >= -15) {
    motorspeed = 0;
  }
  //Serial.print("motorspeed: ");
  //Serial.println(motorspeed);
}

void loop() {
  autonomous();
  //humanControl();
}

void moveForward(){
  setMotorSpeed(motor1,200);
  setMotorSpeed(motor2,200);
}

void moveBackward() {
  setMotorSpeed(motor1,-200);
  setMotorSpeed(motor2,-200);
}

void rotateRight() {
  setMotorSpeed(motor1,-200);
  setMotorSpeed(motor2,200);
}

void rotateLeft() {
  setMotorSpeed(motor1,200);
  setMotorSpeed(motor2,-200);
}

void stopMovement() {
  setMotorSpeed(motor1,0);
  setMotorSpeed(motor2,0);
}

void liftFourBar() {
  setMotorSpeed(motor4,255);
}

void dropFourbar() {
  setMotorSpeed(motor4,0);
}

void suckBalls() {
  setMotorSpeed(motor3,255);
}

void spitBalls() {
  setMotorSpeed(motor3,-255);
}

void setMotorSpeed(AF_DCMotor motor, int motorspeed) {
  if (motorspeed >= 0) {
      motor.run(FORWARD);
      motor.setSpeed(motorspeed);
    } else {
      motor.run(BACKWARD);
      motor.setSpeed(-motorspeed);
    }
}

void autonomous(){
  s1 = digitalRead(2); //IR 3
  s2 = digitalRead(13); //IR 4

  if(s1==1){
    moveForward();
    Serial.println("moving forward");
    
  }else if(s1==0){
    moveBackward();
    Serial.println("moving backward");
  }

//  Serial.print("s1: ");
//  Serial.println(s1);
//  Serial.print("s2: ");
//  Serial.println(s2);

//  if(s1=0 or s2=0){
//    
//  }
//  else{
//    humanControl();
//  }
}

void humanControl(){

  // READ THE PWM FRM RECEIVER
  // pulseIn(pin, value, timeout)
  ch1 = pulseIn(A0, HIGH); // Right Stick LEFT(1170)| CENTRE(1540) | RIGHT(1920)
  ch2 = pulseIn(A1, HIGH); // Right Stick UP(1830)  | CENTRE(1505) | DOWN(1185)  | RIGHT SWITCH +7ms to reading
  ch3 = pulseIn(A2, HIGH); // Left  Stick UP(1850)  |              | DOWN(1190)  | LEFT SWITCH FORCE TO 1005ms
  ch4 = pulseIn(A3, HIGH); // Left  Stick LEFT(1205)| CENTRE(1540) | RIGHT(1930)
  ch5 = pulseIn(A4, HIGH); // Right Knob CW(1005ms) | ACW(2025ms)
  ch6 = pulseIn(A5, HIGH); // Left  Knob CW(1005ms) | ACW(2025ms)

  if (ch1 != 0 or ch2 != 0 or ch3 != 0 or ch4 != 0 or ch5 != 0 or ch6 != 0) {
    float normch1 = normalise(ch1,minch1,maxch1);
    float normch2 = normalise(ch2,minch2,maxch2);
    float normch3 = normalise(ch3,minch3,maxch3);
    float normch4 = normalise(ch4,minch4,maxch4);
    //float normch5 = normalise(ch5,minch5,maxch5);
    //float normch6 = normalise(ch6,minch6,maxch6);
  
    //Set the speeds that are required  
    motor1speed = (normch1+normch2)*255;
    motor2speed = (-normch1+normch2)*255;
    motor3speed = (normch3)*255;
    motor4speed = (normch4)*255;

    //Keep each of the speeds within the speed limit
    speedlimit(motor1speed);
    speedlimit(motor2speed);
    speedlimit(motor3speed);
    speedlimit(motor4speed);

    //CHANGE THE SPEED OF THE MOTORS TO WHATEVER YOU WANT
    setMotorSpeed(motor1,motor1speed);
    setMotorSpeed(motor2,motor2speed);
    setMotorSpeed(motor3,motor3speed);
    setMotorSpeed(motor4,motor4speed);

    //reset speeds
//    ch1 = 0;
//    ch2 = 0;
//    ch3 = 0;
//    ch4 = 0;

    //USE THIS CODE FOR CALIBRATING THE CONTROLLER
    //CHANNELS
    //Serial.print("ch1:");
    //Serial.println(ch1);
    //Serial.print("ch2:");
    //Serial.println(ch2);
    Serial.print("ch3:");
    Serial.println(ch3);
    //Serial.print("ch4:");
    //Serial.println(ch4);
    //Serial.print("ch5:");
    //Serial.println(ch5);
    //Serial.print("ch6:");
    //Serial.println(ch6);
    //SPEEDS
//    Serial.print("Speed 1:");
//    Serial.println(motor1speed);
//    Serial.print("Speed 2:");
//    Serial.println(motor2speed);
//    Serial.print("Speed 3:");
//    Serial.println(motor3speed);
//    Serial.print("Speed 4:");
//    Serial.println(motor4speed);  
    //***MAKE SURE TO REMOVE THE DELAY WHEN NOT IN USE***
//    delay(100);
  }
}
