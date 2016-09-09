#include <ros.h>
#include <challenge2_1/Ultrasonic.h>
#include <challenge2_1/Arm.h>

#include <Servo.h>

//Ultrasonic
#define TIMEOUT 500 //micosecond
#define trig_U       1
#define echo_U   2
#define trig_D       3
#define echo_D   4

//Arm
#define Base 0
#define Lower 1
#define Upper 2
const byte EN[3] = {8, 43, 49};
const byte DIR[3] = {9, 45, 51};
const byte CLK[3] = {10, 47, 53};
#define Clip 7
Servo clip;

//
float dst_sonic( int trig, int echo)
{
    digitalWrite( trig, HIGH);
    delayMicroseconds(500);
    digitalWrite( trig, LOW);
    return pulseIn( echo, HIGH, TIMEOUT)/ 2/ 29.1;
}

void wrtUsonic( challenge2_1::Ultrasonic &msg, int u, int d)
{
    msg.u_dst = u;
    msg.d_dst = d;
}

//ARM control function
int speedMax[3] = {400, 400, 400};
const float speedMin = 5000.0;
float t[3] = {speedMin, speedMin, speedMin};
float ac[3] = { 0.008, 0.008, 0.008};

void StepperControl(int a, boolean b, int c)
{
  float t0 = micros();
  if (b)
  {
    digitalWrite(DIR[c], LOW);
  }
  else
  {
    digitalWrite(DIR[c], HIGH);
  }
  float Ac_interval = (1000 / speedMax[c] - 1000 / speedMin) / ac[c];
  if (Ac_interval > (a / 2))
  {
    Ac_interval = a / 2;
  }
  int Cs_interval = a - 2 * Ac_interval;
  for (int i = 0; i < Ac_interval ; i = i + 1)
  {
    digitalWrite(CLK[c], HIGH);
    delayMicroseconds(t[c]);
    digitalWrite(CLK[c], LOW);
    delayMicroseconds(t[c]);
    double buff = 1000 / t[c];
    buff = buff + ac[c];
    t[c] = 1 / buff * 1000;
    Serial.println(t[c]);
  }
  for (int i = 0; i < Cs_interval ; i = i + 1)
  {
    digitalWrite(CLK[c], HIGH);
    delayMicroseconds(t[c]);
    digitalWrite(CLK[c], LOW);
    delayMicroseconds(t[c]);
    Serial.println(t[c]);
  }
  for (int i = 0; i < Ac_interval ; i = i + 1)
  {
    digitalWrite(CLK[c], HIGH);
    delayMicroseconds(t[c]);
    digitalWrite(CLK[c], LOW);
    delayMicroseconds(t[c]);
    double buff = 1000 / t[c];
    buff = buff - ac[c];
    t[c] = 1 / buff * 1000;
    //nh.loginfo(" t[ %d] = %f", c, t[c]);
  }
  //nh.loginfo(" micros() - t0 = %f", micros() - t0);
}

void wrtArm( const challenge2_1::Arm &msg)
{
  StepperControl( abs( msg.baseSteps), msg.baseSteps >  0, Base);
  StepperControl( abs( msg.lowerSteps), msg.lowerSteps >  0, Lower);
  StepperControl( abs( msg.upperSteps),msg.upperSteps  >  0, Upper);
  clip.write( msg.clipAngular);
}

//ROS Init varible
ros::NodeHandle nh;

challenge2_1::Arm arm;
ros::Subscriber< challenge2_1::Arm > subArm("Arduino_Arm", wrtArm );

challenge2_1::Ultrasonic usonic;
ros::Publisher pubUltrasonic( "Arduino_Ultrasonic", & usonic);

void setup()
{
   //Ultrasonic
   pinMode( trig_U, OUTPUT);
   pinMode( echo_U, INPUT);
   pinMode( trig_D, OUTPUT);
   pinMode( echo_D, INPUT);
   
   //Arm
  pinMode(EN[2], OUTPUT);
  pinMode(CLK[2], OUTPUT);
  pinMode(DIR[2], OUTPUT);
  pinMode(EN[1], OUTPUT);
  pinMode(CLK[1], OUTPUT);
  pinMode(DIR[1], OUTPUT);
  pinMode(EN[0], OUTPUT);
  pinMode(CLK[0], OUTPUT);
  pinMode(DIR[0], OUTPUT);

  //Arm Clip
  clip.attach( Clip);
 
  //ROS
  nh.initNode();
  nh.subscribe(subArm);
  nh.advertise( pubUltrasonic);
}

void loop()
{
    wrtUsonic( usonic, dst_sonic( trig_U, echo_U), dst_sonic( trig_D, echo_D));
    pubUltrasonic.publish( &usonic);
    
    nh.spinOnce();
}
