#include <ros.h>
#include <challenge1/Motor.h>
#include <challenge1/Ultrasonic.h>

#define leftwheel0 5
#define leftwheel1 6
#define rightwheel0 10
#define rightwheel1 11

ros::NodeHandle  nh;

void writeMotor( const challenge1::Motor &msg){
  analogWrite( leftwheel0, msg.l_clk);
  analogWrite( leftwheel1, msg.l_aclk);
  analogWrite( rightwheel0, msg.r_clk);
  analogWrite( rightwheel1, msg.r_aclk);
}

void pubUsonic( challenge1::Ultrasonic &msg, float f, float h, float r, float l)
{
   msg.f_dst = f;
   msg.h_dst = h;
   msg.r_dst = r;
   msg.l_dst = l;
}

ros::Subscriber< challenge1::Motor > submotor("Arduino_Motor", writeMotor );

challenge1::Ultrasonic usonic;
ros::Publisher pubUtrasonic("Arduino_Ultrasonic", &usonic);

void setup()
{
  pinMode( leftwheel0, OUTPUT);
  pinMode( leftwheel1, OUTPUT);
  pinMode( rightwheel0, OUTPUT);
  pinMode( rightwheel1, OUTPUT);
  nh.initNode();
  nh.subscribe(submotor);
  nh.advertise(pubUtrasonic);
}

void loop()
{
  pubUsonic( usonic, 1.2, 2.3, 3.4, 4.5);
  pubUtrasonic.publish( &usonic);
  nh.spinOnce();
  delay(100);
}
