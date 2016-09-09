#include <ros.h>
#include <challenge1/Motor.h>
#include <challenge1/Ultrasonic.h>
#include <challenge1/IR_trigger.h>

//Motor
#define leftwheel0 10
#define leftwheel1 11
#define rightwheel0 8
#define rightwheel1 9

//Ultrasonic
#define TIMEOUT 500 //microsecond
#define trig_R 28
#define echo_R 29
#define trig_C 32
#define echo_C 33
#define trig_L 36
#define echo_L 37
#define trig_RC 22
#define echo_RC 23
#define trig_LC 24
#define echo_LC 25


//IR_trigger
#define IR_trig_R 2
#define IR_trig_L 4
#define IR_trig_C 3
#define IR_trig_RR 6
#define IR_trig_LL 5


float dst_sonic(int trig,int echo) 
{
  float duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(500);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, TIMEOUT);
  return duration/2/29.1;  
}

void writeMotor( const challenge1::Motor &msg){
  analogWrite( leftwheel0, msg.l_clk);
  analogWrite( leftwheel1, msg.l_aclk);
  analogWrite( rightwheel0, msg.r_clk);
  analogWrite( rightwheel1, msg.r_aclk);
}

int ir2trig( int ir_in)
{
  return ir_in;
}

void wrtUsonic( challenge1::Ultrasonic &msg, float f, float h, float r, float l, float rf, float lf)
{
   msg.f_dst = f;
   msg.h_dst = h;
   msg.r_dst = r;
   msg.l_dst = l;
   msg.rf_dst = rf;
   msg.lf_dst = lf;
}

void wrtIR_trig( challenge1::IR_trigger &msg , int c, int r, int l, int rr, int ll)
{
  msg.trig_c = c;
  msg.trig_r = r;
  msg.trig_l = l;
  msg.trig_rr = rr;
  msg.trig_ll = ll;
}

ros::NodeHandle  nh;

ros::Subscriber< challenge1::Motor > submotor("Arduino_Motor", writeMotor );

challenge1::Ultrasonic usonic;
ros::Publisher pubUtrasonic("Arduino_Ultrasonic", &usonic);

challenge1::IR_trigger ir_trig;
ros::Publisher pubIR_trigger("Arduino_IR_trigger", &ir_trig);

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
  pinMode(trig_RC, OUTPUT);
  pinMode(echo_RC, INPUT);
  pinMode(trig_C, OUTPUT);
  pinMode(echo_C, INPUT);
  pinMode(trig_LC, OUTPUT);
  pinMode(echo_LC, INPUT);
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
  //wrtUsonic( usonic, dst_sonic(trig_C,echo_C), 0, dst_sonic(trig_R,echo_R), dst_sonic(trig_L,echo_L), dst_sonic(trig_RC,echo_RC), dst_sonic(trig_LC,echo_LC));
  //pubUtrasonic.publish( &usonic);

  wrtIR_trig( ir_trig, ir2trig( digitalRead( IR_trig_C)), ir2trig( digitalRead( IR_trig_R)), ir2trig( digitalRead( IR_trig_L)), ir2trig( digitalRead( IR_trig_RR)), ir2trig( digitalRead( IR_trig_LL)));
  pubIR_trigger.publish( &ir_trig);

  nh.spinOnce();
  //delay(100);
}
