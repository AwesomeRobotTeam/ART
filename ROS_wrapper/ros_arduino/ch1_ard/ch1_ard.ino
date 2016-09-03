#include <ros.h>
#include <challenge1/Motor.h>
#include <challenge1/Ultrasonic.h>
#include <challenge1/IR_trigger.h>

//Motor
#define leftwheel0 8
#define leftwheel1 9
#define rightwheel0 10
#define rightwheel1 11

//Ultrasonic
#define trig_R 2
#define echo_R 3
#define trig_C 4
#define echo_C 5
#define trig_L 6
#define echo_L 7

//IR_trigger
#define IR_trig_R 52
#define IR_trig_L 53
#define IR_trig_C 51


float dst_sonic(int trig,int echo) 
{
  float duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  return duration/2/29.1;  
}

void writeMotor( const challenge1::Motor &msg){
  analogWrite( leftwheel0, msg.l_clk);
  analogWrite( leftwheel1, msg.l_aclk);
  analogWrite( rightwheel0, msg.r_clk);
  analogWrite( rightwheel1, msg.r_aclk);
}

void wrtUsonic( challenge1::Ultrasonic &msg, float f, float h, float r, float l)
{
   msg.f_dst = f;
   msg.h_dst = h;
   msg.r_dst = r;
   msg.l_dst = l;
}

void wrtIR_trig( challenge1::IR_trigger &msg , int c, int r, int l)
{
  msg.trig_c = c;
  msg.trig_r = r;
  msg.trig_l = l;
}

ros::NodeHandle  nh;

ros::Subscriber< challenge1::Motor > submotor("Arduino_Motor", writeMotor );

challenge1::Ultrasonic usonic;
ros::Publisher pubUtrasonic("Arduino_Ultrasonic", &usonic);

challenge1::IR_trigger ir_trig;
ros::Publisher pubIR_trigger("Arduino_IR_trigger_", &ir_trig);

void setup()
{
  //Motor
  pinMode( leftwheel0, OUTPUT);
  pinMode( leftwheel1, OUTPUT);
  pinMode( rightwheel0, OUTPUT);
  pinMode( rightwheel1, OUTPUT);
  //Ultrasonic
  pinMode(trig_R, OUTPUT);
  pinMode(echo_R, INPUT);
  pinMode(trig_C, OUTPUT);
  pinMode(echo_C, INPUT);
  pinMode(trig_L, OUTPUT);
  pinMode(echo_L, INPUT);
  //IR_Tracker
  pinMode(IR_trig_R, INPUT);
  pinMode(IR_trig_L, INPUT);
  pinMode(IR_trig_C, INPUT);

  //ROS
  nh.initNode();
  nh.subscribe(submotor);
  nh.advertise(pubUtrasonic);
  nh.advertise( pubIR_trigger);
}

void loop()
{
  wrtUsonic( usonic, dst_sonic(trig_C,echo_C), 0, dst_sonic(trig_R,echo_R), dst_sonic(trig_L,echo_L));
  pubUtrasonic.publish( &usonic);

  wrtIR_trig( ir_trig, digitalRead( IR_trig_C), digitalRead( IR_trig_R), digitalRead( IR_trig_L));
  pubIR_trigger.publish( &ir_trig);

  nh.spinOnce();
  delay(100);
}
